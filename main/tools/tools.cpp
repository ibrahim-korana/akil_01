

void list_lamps(void)
{
    lamps_t *target = lamp_list;
    ESP_LOGI("MAIN","     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    ESP_LOGI("MAIN","     LAMPS");
    ESP_LOGI("MAIN","     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"); 
    while(target)
      {
        ESP_LOGI("MAIN","     %3d %-20s %d %-20s %d %d %s", 
              target->id,
              target->name,
              target->local,
              target->text,
              target->width,
              target->height,
              target->iname
              );
          target=(lamps_t *)target->next;
      }
    ESP_LOGI("MAIN","     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");  
}


void add_lamps(lamps_t *lc )
{
  lc->next = (lamps_t *)lamp_list;
  lamp_list = lc;
}

void read_lamps(void)
{
    const char *name1="/config/lamp.json";
    if (disk.file_search(name1))
      {
        int fsize = disk.file_size(name1); 
        char *buf = (char *) malloc(fsize+5);
        if (buf==NULL) {ESP_LOGE("MAIN", "memory not allogate"); return;}
        FILE *fd = fopen(name1, "r");
        if (fd == NULL) {ESP_LOGE("MAIN", "%s not open",name1); return;}
        fread(buf, fsize, 1, fd);
        fclose(fd);
        DynamicJsonDocument doc(3000);
        DeserializationError error = deserializeJson(doc, buf);

        if (error) {
          ESP_LOGE("MAIN","deserializeJson() failed: %s",error.c_str());
          return;
        }
        for (JsonObject function : doc["lamps"].as<JsonArray>()) {
          const char* a_name = function["name"];
          const char* i_name = function["rname"];
          const char* a_text = function["text"];
          int a_id = function["id"];
          int a_local = function["local"];           
          int a_w = function["width"]; 
          int a_h = function["height"]; 
          int a_nt = function["new_track"];          
          if (a_w==0) a_w=90;
          if (a_h==0) a_h=90;
         
          lamps_t *bb0 = (lamps_t *)calloc(1, sizeof(lamps_t)); 
          strcpy(bb0->name,a_name);
          strcpy(bb0->text,a_text);
          if (i_name!=NULL) {
              strcpy(bb0->iname,i_name);
          } else strcpy(bb0->iname,"");
          bb0->id = a_id;
          bb0->local = a_local;
          bb0->width = a_w;
          bb0->height = a_h;
          bb0->new_track = a_nt;
          add_lamps(bb0);
        }      
        doc.clear();                       
        free(buf);
      } else ESP_LOGW("MAIN", "Config File BULUNAMADI !...");
   return;   
}


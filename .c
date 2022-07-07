#pragma strict_types

static variables inherit "room/room";

void create() {
  room::create();

  set_light(1);
  set_outdoors(0);
  
  set_short("Maker's Forge");
  set_long("Maker's Forge");
    
  set_sound("In the distance you can hear someone singing.");
  set_smell("You smell iron and wine.");
  
  add_senese( ({"anvil", "large anvil", "fairly large anvil"}), ([
    "item":  "The anvil is rather large.",
    "noget": "You cannot possibly lift that up!",
    "smell": "It smells of hot iron.",
    "touch": "It feels warm and well used. Kinda.",
    "sound": "It sizzles.",
    "taste": "The anvil burns your tongue!"]));
  

  clone_object("/w/tueblo/toolbox/fire")->move(this_object());
  add_senses( ({"fire", "flame", "hot flame", "white hot flame", "white flame"}), ([
    "item":  "The flames near the anvil are white hot, you could forge nearly anything with that kind of heat.",
    "noget": "You would burn yourself even getting near the flames!",
    "smell": "You singe your nose hair!",
    "touch": "That would burn you!",
    "sound": "You cannot hear anything more than what you would expect from a flame.",
    "taste": "Why on Oerthe would you attempt to taste a WHITE HOT FLAME?" ]));
}


/*

#include <daemons.h>
#define TP this_player()
#define MYRACE "martian"
//#define MYCLASS "w/dristan/druids/room/grove"
#define ME "maker"
#define SAVE "/w/"+ME+"/private/workroom"
#define LOG "/w/"+ME+"/private/log/workroom_log"
#define X_COORD 41
#define Y_COORD 25
#define LOG_PATH 0 // keys in the mapping logs
#define LOG_DATE 1

inherit "/w/maker/tools/inv_saver";
mapping banished;

private nomask mapping logs;

varargs nomask int check_log(string arg);
private nomask int secure();

static string *keep_alives;

int keep_alive();

void create() {
  room::create();
  if(is_clone()){destruct(this_object()); return;}
  
  if(!restore_object(SAVE)) {
    banished = ([]);
    logs = ([]);
  }
  
  keep_alives = ({});
  
  add_item("sign", "You could 'read sign'.");
}

void init() { 
  room::init();
  inv_saver::init();
  add_action("read","read");
  add_action("map",  "map");
 
  if(secure()) {
    this_player()->remove_hook("move_player","save_me");
    this_player()->add_hook("move_player", "save_me");
    add_action("banish", "banish");
    add_action("unbanish", "unbanish");
    add_action("add_log", "addlog");
    add_action("remove_log", "removelog");
    add_action("check_log", "checklog");
    keep_alive();
    if((string)this_player()->query_race() != MYRACE) {
      this_player()->set_race(MYRACE);
      writef("Race set to: "+MYRACE+".\n");
    }
    if((string)this_player()->query_class() != MYCLASS)
    {
      this_player()->set_class(MYCLASS);
      writef("Class set to: "+MYCLASS+".\n");
    }

    if(present("bear's claw", find_player(ME)))
      "w/taver/guild/room/guildhall"->set_member_spirit("maker","yeti");
    check_log();
  }
  else
    if(this_player()->query_is_player()) {
      if(this_player()->query_invis()) {
        if(find_player(ME))
          tell_object(find_player(ME), "NOTICE: ("+
          capitalize((string)this_player()->query_real_name())+
          ") entered your workroom.\n");
      }
      log_file(LOG,"["+ctime(time())+"]"+" "+
        capitalize((string)this_player()->query_real_name())+
        " entered your workroom.\n");
    }
}

void reset() {
  room::reset();
}

void long(string arg) {
  if(!secure() && environment(this_interactive()) != this_object()) {
    tell_object(this_interactive(),
      "You cough and sputter as smoky flames bring tears to your eyes.\n");
    if(find_player(ME))
      tell_object(find_player(ME), "Smoke from the forge causes "+
        capitalize((string)this_interactive()->query_real_name())+
        " to cough and sputter.\n");
    log_file(LOG, "["+ctime(time())+"] "+
      capitalize((string)this_interactive()->query_real_name())+
      " peeked into your room.\n");
    return;
  }
  if(!arg)
    writef("This is "+capitalize(ME)+"'s Forge. You see a fairly large "
      "anvil near a white hot flame. Perhaps you should keep your distance. "
      "There is a sign hanging on the wall.");
  room::long(arg);
}

void exit() {
  if(!secure() && this_player()->query_is_player()
    && this_player()->query_invis() && find_player(ME))
    tell_object(find_player(ME), "NOTICE: ("+
      capitalize((string)this_player()->query_real_name())+
      ") exited your workroom.\n");
}

void save_me(string arg) {
  if(!secure() && environment(find_player("maker")) ==
    find_object("/w/bytre/death/death"))
    find_player("maker")->move(this_object());
}

mixed query_property(string str) {
  return this_player() && secure() ? 0 : 1;
}

int banish(string str) {
  int i;
  string a;
  string *list, *lines;
  if(!str) {
    write("Banish who?\n");
    return 1;
  }
  if(str == "list") {
    lines = ({});
    list = m_indices(banished);
    for(i=0;i<sizeof(list);i++) {
      lines += ({capitalize(list[i])+" for "+banished[list[i]]+"."});
    }
    if(sizeof(lines))
      this_player()->more(lines);
    else
      write("You've recorded no names.\n");
    return 1;
  }
  sscanf(str, "%s %s", str, a);
  "/secure/banish"->banish(str);
  if(!a)
    a = "no reason";
  banished[str] = a;
  save_object(SAVE);
  return 1;
}

int unbanish(string str) {
  if(!str) {
    write("Unbanish who?\n");
    return 1;
  }
  "/secure/banish"->do_unbanish(str);
  banished -= ([str]);
  save_object(SAVE);
  return 1;
}

int keep_alive() {
  string name;
  
  name = (string)this_player()->query_real_name();
  keep_alives = keep_alives - ({name}) + ({name});
  while(remove_call_out("keep_them_alive") > -1);
  call_out("keep_them_alive", 60);
}

void keep_them_alive() {
  int i;
  object player;
  string *dead;
  
  dead = ({});
  
  for(i=0; i<sizeof(keep_alives); i++) {
    player = find_player(keep_alives[i]);
    if(player && interactive(player))
      tell_object(player, "\nkeep_my_connection_alive\n");
    else
      dead += ({keep_alives[i]});
  }
  keep_alives -= dead;
  
  if(sizeof(keep_alives))
    call_out("keep_them_alive", 60);
}

int read(string what) {
  notify_fail("Read what?\n");
  if(!what) {
    return 0;
  } else {
    if(what == "sign") {
      if(this_player()->query_asciiart()) {
        this_player()->prefomat(1);
        writef(
          "+------------------------------+ \n"
          "| "+capitalize(ME)+"'s Forging Queue:       | \n"
          "|                              | \n"
          "| Installed:                   | \n"
          "| <none>                       | \n"
          "|                              | \n"
          "| QC:                          | \n"
          "| frosty_cmd                   | \n"
          "|                              | \n"
          "| Balance:                     | \n"
          "| <none>                       | \n"
          "|                              | \n"
          "| World:                       | \n"
          "| <none>                       | \n"
          "|                              | \n"
          "| World pre-approved:          | \n"
          "| grish <redwood fishing area> | \n"
          "| tortoise <psion clan>        | \n"
          "|                              | \n"
          "| TBA:                         | \n"
          "| valina <Vajalorian smith>    | \n"
          "+------------------------------+ \n"
          "\n"
          "You could get info with 'read <project>' \n"
          "or your could 'map' it.                  \n"
          "\n"
        );
        this_player()->preformat(0);
      } else {
        writef(capitalize(ME)+"'s Forging Queue:\n");
        writef("Installed: <none>\n");
        writef("QC: frosty_cmd\n");
        writef("Balance: <none>\n");
        writef("World: <none>\n");
        writef("World approved: grish, tortoise clan\n");
        writef("TBA: valina\n");
      }
      return 1;
    }
    if(what == "grish") {
      this_player()->more("w/"ME"/grish/doc/world.info.grish");
      return 1;
    }
    if(what == "valina") {
      this_player()->more("w/"ME"/valina/doc/world.info.valina");
      return 1;
    }
    if(what == "tortoise") {
      this_player()->more("w/"ME"/tortoise/doc/world.info.tortoise");
      return 1;
    }
  }
  return 0;
}

int map(string what) {
  int x;
  int y;
  notify_fail("Map what?\n");
  if(what) {
    if(what == "grish") {
      x = (int)("w/"ME"/grish/room/baseroom.c"->query_x_coord());
      y = (int)("w/"ME"/grish/room/baseroom.c"->query_y_coord());
    }
    if(what == "valina") {
      x = (int)("w/"ME"/valina/room/baseroom.c"->query_x_coord());
      y = (int)("w/"ME"/valina/room/baseroom.c"->query_y_coord());
    }
    if(what == "tortoise") {
      x = (int)("w/"ME"/tortoise/room/baseroom.c"->query_x_coord());
      y = (int)("w/"ME"/tortoise/room/baseroom.c"->query_y_coord());
    }
    "/room/map/mapbase"->create();
    "/room/map/mapbase"->set_size(17);
    "/room/map/mapbase"->show_map(0,x,y);

    write("Coordinates x: " + (string) x +
       ", y: " + y + ".\n");
    return 1;
  }
  return 0;
}

nomask int add_log(string arg) {
  string thepath;
  if(!secure())
    return 0;
  if(!arg || sscanf(arg, "%s %s", arg, thepath) != 2) {
    writef("Usage: 'addlog <logname> <logpath>'\n");
    return 1;
  }
  thepath = (string)TP->get_custom_path((string)TP->query_path(), thepath);
  logs += ([arg: thepath; 0]);
  writef("LOG: " + arg + " linked to " + thepath + ".");
  save_object(SAVE);
  return 1;
}

nomask int remove_log(string arg) {
  if(!secure())
    return 0;
  if(!arg) {
    writef("Usage: 'removelog <logname>'\n");
    return 1;
  }
  logs -= ([arg]);
  write("LOG: " + arg + " removed.\n");
  save_object(SAVE);
  return 1;
}

varargs nomask int check_log(string arg) {
  int i;
  string *str;
  if(secure()) {
    if(!arg) {
      str = m_indices(logs);
      for(i=0; i<sizeof(str); i++)
        if(logs[str[i], LOG_DATE] < file_date(logs[str[i], LOG_PATH])) {
          write(str[i] + " ");
        }
      return 1;
    }
    if(arg == "list") {
      str = m_indices(logs);
      for(i=0; i<sizeof(str); i++)
        printf("%-15s %s\n", str[i], logs[str[i]]); 
    } else if(member(logs, arg)) {
      if(file_size(logs[arg, LOG_PATH]) != -1) {
        tail(logs[arg, LOG_PATH]);
        logs[arg, LOG_DATE] = time();
      } else
        writef("LOG: " + arg + " does not exist.");
    }
    save_object(SAVE);
    return 1;
  }
  return 0;
}

private nomask int secure() {
  if((string)getuid(this_player()) == ME &&
    (string)this_player()->query_real_name() == ME)
    return 1;
  return 0;
}

int query_snoop_protected(){ return 1; } 
int query_x_coord(){ return X_COORD; }
int query_y_coord(){ return Y_COORD; }

*/

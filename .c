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
  
  add_senses( ({"anvil", "large anvil", "fairly large anvil"}), ([
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

void init() {
  ::init();
  
  if((string)this_player()->query_real_name() == "maker") {
    // initialize keep_alive call
    while(remove_call_out("keep_alive") > -1);
    call_out("keep_alive", 60);
    
    // set bear spirit
    if(present("bear's claw", this_player()))
      "w/taver/guild/room/guildhall"->set_member_spirit("maker","yeti");
  }
}
  
void keep_alive() {
  object player;

  player = find_player("maker");

  if(player && interactive(player)) {
    if(query_idle(player) > 60)
      tell_object(player, ">\n");
    else
      call_out("keep_alive", 60);
  }
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

int query_snoop_protected(){ return 1; } 
int query_x_coord(){ return X_COORD; }
int query_y_coord(){ return Y_COORD; }

*/

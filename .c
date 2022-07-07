#pragma strict_types

#include <guilds.h>

#define ME "maker"
#define MYRACE "martian"
#define MYSPIRIT "yeti"
#define X_COORD 41
#define Y_COORD 25

static variables inherit "room/room";

void create() {
  room::create();

  if(is_clone()){destruct(this_object()); return;}

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
  object player_object;
  string player_name;

  room::init();

  player_object = (object)this_interactive();
  player_name = (string)player_object->query_real_name();
  
  if(player_name != ME)
    return;
  
  // initialize keep_alive call
  while(remove_call_out("keep_alive") > -1);
  call_out("keep_alive", 60);
  
  // set bear spirit
  if(present("bear's claw", player_object) &&
  (string)BEAR->query_member_spirit(player_name) != MYSPIRIT) {
      BEAR->set_member_spirit(player_name, MYSPIRIT);
      writef("Bear Spirit set to: "+MYSPIRIT);
  }

  // set race
  if((string)player_object->query_race() != MYRACE) {
    player_object->set_race(MYRACE);
    writef("Race set to: "+MYRACE);
  }

}
  
void keep_alive() {
  object player_object;

  player_object = find_player("maker");

  if(player_object && interactive(player_object)) {
    if(query_idle(player_object) > 60)
      tell_object(player_object, ">\n");
    else
      call_out("keep_alive", 60);
  }
}

int query_snoop_protected(){ return 1; } 
int query_x_coord(){ return X_COORD; }
int query_y_coord(){ return Y_COORD; }

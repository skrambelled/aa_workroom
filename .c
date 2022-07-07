#pragma strict_types

#define ME "maker"
#define MYRACE "martian"
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
  room::init();
  
  if((string)this_interactive()->query_real_name() == ME) {
    // initialize keep_alive call
    while(remove_call_out("keep_alive") > -1);
    call_out("keep_alive", 60);
    
    // set bear spirit
    if(present("bear's claw", this_interactive())) {
      "w/taver/guild/room/guildhall"->set_member_spirit(ME, "yeti");
      writef("Bear Spirit set to: yeti.");
    }

    // set race
    if((string)this_player()->query_race() != MYRACE) {
      this_player()->set_race(MYRACE);
      writef("Race set to: "+MYRACE);
    }
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

int query_snoop_protected(){ return 1; } 
int query_x_coord(){ return X_COORD; }
int query_y_coord(){ return Y_COORD; }

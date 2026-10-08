// The Traffic Light — actuated
// As shown in the reel
// Generated from the episode; edits here are overwritten. See the README.

void serve(Loop& loop) {
  if (!loop.has_metal()) return;
  road = YELLOW; wait(3);
  road = RED;    wait(1);
  side = GREEN;  wait(8);
  side = YELLOW; wait(3);
  side = RED;    wait(1);
  road = GREEN;
}

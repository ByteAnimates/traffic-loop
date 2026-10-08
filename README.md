# The Traffic Light

**actuated**

The working code from the [@ByteAnimates](https://www.facebook.com/ByteAnimates) reel.

```bash
c++ -std=c++17 -O2 -o main main.cpp
./main
```

No dependencies. Any C++17 compiler.

### `signal.cpp`

```cpp
void serve(Loop& loop) {
  if (!loop.has_metal()) return;
  road = YELLOW; wait(3);
  road = RED;    wait(1);
  side = GREEN;  wait(8);
  side = YELLOW; wait(3);
  side = RED;    wait(1);
  road = GREEN;
}
```

### Files

| | |
| --- | --- |
| `main.cpp` | build and run this — the demo, with real inputs and real output |
| `signal.cpp` | the reel snippet, generated from the episode |

---

The snippet above is generated from the video itself — what you read is byte-for-byte
what was typed on screen. A fix to it belongs in the episode, so open an issue and the
next reel carries it. Everything else here is hand-written and welcome as a pull request.

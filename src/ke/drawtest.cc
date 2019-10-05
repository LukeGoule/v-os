#include <drawtest.h>
#include <kernel.h>
#include <maths.h>
#include <system.h>

/*
  This file is purely dedicated to running graphical and mathematical tests.
  It's useless as it doesn't actually do anything productive with the OS.
*/

#define putpixel(x,y) vga.glPlotPixel(x,y,0xFFFFFF)

void drawcircle(int x0, int y0, int radius)
{
    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
        putpixel(x0 + x, y0 + y);
        putpixel(x0 + y, y0 + x);
        putpixel(x0 - y, y0 + x);
        putpixel(x0 - x, y0 + y);
        putpixel(x0 - x, y0 - y);
        putpixel(x0 - y, y0 - x);
        putpixel(x0 + y, y0 - x);
        putpixel(x0 + x, y0 - y);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void draw_line(int x1, int y1, int x2, int y2, int colour) {
  int dx = x2 - x1;
  int dy = y2 - y1;
  int ox = 0, oy = 0;
  
  for (ox = x1; ox < x2; ox++) {
    oy = y1 + dy * (ox - x1) / dx;
    vga.glPlotPixel(ox, oy, colour);
  }
}

void DoDrawTest() {
  vga.glSquare(0,0,1024,768, 0x0); // Clear the entire screen.
  
  /*
  local r = math.abs(math.sin(time * 0.5) * 255);
  local g = math.abs(math.sin(time * 0.5 + 2) * 255);
  local b = math.abs(math.sin(time * 0.5 + 4) * 255);
  */
  
  int shift = 0;
  int r = 255, g = 0,b = 0;
  while (1) {
    if(r > 0 && b == 0){
      r--;
      g++;
    }
    if(g > 0 && r == 0){
      g--;
      b++;
    }
    if(b > 0 && g == 0){
      r++;
      b--;
    }

    vga.glSquare(0,0,vga.w,vga.h, byteColours(r,g,b));
    drawcircle(vga.w / 2, vga.h / 2, 100);
    shift+=1;
  }

  if (inportb(0x64) & 0x1) {
    return;
  }
}

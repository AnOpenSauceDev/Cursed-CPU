// gpu presents using tiles. There is an independent sprite and tile layer. This has a 160x160 resolution 10 tiles each way

/**
 * 
*/

// use pointers for repeating tiles


typedef struct Pixel
{
    unsigned char color; // 0 = don't render, the other 255 values steps of 3 in R,G,B, with 1 = black.
}Pixel;

typedef struct Tile
{
    Pixel pixels[256]; // every 16px horizontal, we go down by one pixel.
}Tile;

// make GPU flexible, frame is hardcoded.
int render(){
    Tile background[100];

   unsigned char pixels[256] =
   {
    0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,               
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,                     
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,                    
    };

    for(int i = 0; i < 100; i++ ){
        for (int x = 0; x < 256; x++)
        {
            background[i].pixels[x].color = pixels[x];
        }
        
  
    }

    

}
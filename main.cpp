#include <SDL2/SDL.h>

#include <iostream>
int WIDTH = 640;
int HEIGHT = 480;
int thread_exit = 0;
int fresh(void *data){
  while (thread_exit==0){
    SDL_Event event;
    event.type  = SDL_DISPLAYEVENT;
    SDL_PushEvent(&event);
    SDL_Delay(40);
  }
}

int main(int argc, char *argv[]) {
  //初始化
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    std::cout << "fail to init sdl " << SDL_GetError() << std::endl;
    return -1;
  }
  //窗口
  SDL_Window *p_window = SDL_CreateWindow("yuv player", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, WIDTH,
                                          HEIGHT, SDL_WINDOW_RESIZABLE);
  if (p_window == NULL) {
    std::cout << "fail to create window" << std::endl;
    return -1;
  }
  //渲染
  SDL_Renderer *p_render = SDL_CreateRenderer(p_window, -1, 0);
  if (p_render == NULL){
    std::cout << "fail to create render" << SDL_GetError() << std::endl;
    return -1;
  }
  //纹理
  SDL_Texture *p_texture = SDL_CreateTexture(p_render,SDL_PIXELFORMAT_IYUV,SDL_TEXTUREACCESS_STREAMING,WIDTH,HEIGHT);
  if (p_texture == NULL){
    std::cout << "fail to create  texture " << SDL_GetError() << std::endl;
    return -1;
  }
  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.w = WIDTH;
  rect.h = HEIGHT;
  //读取文件
  FILE *fp = fopen(argv[1],"rb+");
  uint8_t  buffer[WIDTH*HEIGHT*12/8];
  SDL_Thread *p_refresh = SDL_CreateThread(fresh,NULL,NULL);
  while(true){
    SDL_Event event;
    SDL_WaitEvent(&event);
    if(event.type == SDL_DISPLAYEVENT) {
      //读取yuv数据
      if (fread(buffer, 1, WIDTH * HEIGHT * 12 / 8, fp) !=
          WIDTH * HEIGHT * 12 / 8) {
        fseek(fp, 0, SEEK_SET);
        fread(buffer, 1, WIDTH * HEIGHT * 12 / 8, fp);
      }
      //更新和播放
      SDL_UpdateTexture(p_texture, NULL, buffer, WIDTH);

      SDL_RenderClear(p_render);
      SDL_RenderCopy(p_render, p_texture, NULL, &rect);
      SDL_RenderPresent(p_render);
      SDL_Delay(40);
    }else if (event.type == SDL_WINDOWEVENT){
      SDL_GetWindowSize(p_window,&WIDTH,&HEIGHT);
    }
    else if (event.type  == SDL_QUIT){
      break;
    }

  }


  fclose(fp);
  SDL_DestroyTexture(p_texture);
  SDL_DestroyRenderer(p_render);
  SDL_DestroyWindow(p_window);
  SDL_Quit();
  return 0;
}
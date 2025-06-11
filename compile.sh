emcc *.cpp -o kalkulator.html \
  -s USE_SDL=2 \
  -s USE_SDL_TTF=2 \
  --preload-file assets \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s ASYNCIFY \
  -O3
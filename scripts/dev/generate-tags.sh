ctags -R \
  --c-kinds=+p \
  --fields=+iaS \
  --extras=+q \
  --exclude=build \
  --exclude=.git \
  .

find . -name '*.c' -o -name '*.h' -o -name '*.S' > cscope.files
cscope -b -q -k

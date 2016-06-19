template <typename T>
class BlockStrided {
   T* array;
   int stride, block;
public:
   BlockStrided<T>(T* _array, int _stride, int _block):
      array(_array), stride(_stride), block(_block) {}
   T& operator[](int index) {
      int bnum = index / block;
      return array[bnum * stride + index % block];
   }
   BlockStrided<T> operator+(int offset) {
      return BlockStrided<T>(
            array + (offset/block*stride + offset%block),
            stride, block);
   }
};

template <typename T, int block, int stride>
class CBlockStrided {
   T* array;
public:
   CBlockStrided<T, block, stride>(T* _array):
		array(_array) {}
   T& operator[](int index) {
      int bnum = index / block;
      return array[bnum * stride + index % block];
   }
   CBlockStrided<T, block, stride> operator+(int offset) {
      return CBlockStrided<T, block, stride>(
            array + (offset/block*stride + offset%block));
   }
};

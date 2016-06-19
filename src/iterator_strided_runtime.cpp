template <typename T>
class Strided {
   T* array;
   int stride;
public:
   Strided<T>(T* _array, int _stride):
		array(_array), stride(_stride) {}
   T& operator[](int index) {
      return array[index * stride];
   }
   Strided<T> operator+(int offset) {
      return Strided<T>(array + offset * stride, stride);
   }
};

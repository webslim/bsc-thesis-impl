template <typename T, int stride>
class CStrided {
   T* array;
public:
   CStrided<T, stride>(T* _array):
		array(_array) {}
   T& operator[](int index) {
      return array[index * stride];
   }
   CStrided<T, stride> operator+(int offset) {
      return CStrided<T, stride>(array + offset * stride);
   }
};

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

namespace zich {
    class Matrix {
        public:
            
            Matrix(const std::vector<double>& values, int height, int width);
            ~Matrix();
            
            // get fuctions
            int get_width() const;
            int get_height() const;
            double get_cell(int x, int y) const;
            vector<double> get_vector() const;
            // set functions
            void set_cell(int x, int y, double value);
            // checks
            bool same_size(const Matrix& mat) const;

            //+++++++++++++++++++++++++++++++++++++++
            // mat + mat
            friend Matrix operator+(Matrix lMat, const Matrix& Matr);
            // += operator
            Matrix& operator+=(const Matrix& rMat);
            // unary + operator
            Matrix& operator+();
            // ++mat
            Matrix& operator++();
            // mat++
            Matrix operator++(int);
            
            //----------------------------------------
            // mat - mat
            friend Matrix operator-(Matrix lMat, const Matrix& rMat);
            // -= operator
            Matrix& operator-=(const Matrix& rMat);
            // unary - operator
            friend Matrix operator-(const Matrix& mat);
            // --mat
            Matrix& operator--();
            // mat--
            Matrix operator--(int);
            
            //****************************************
            // mat * mat
            friend Matrix operator*(Matrix lMat, const Matrix& rMat);
            // mat * scalar
            friend Matrix operator*(Matrix lMat, double scalar);
            // scalar * mat
            friend Matrix operator*(double scalar,Matrix rMat);
            // *=
            Matrix& operator*=(double scalar);
            // *=
            Matrix& operator*=(const Matrix& rMat);

            //<><><><><><><><><><><><><><><><><><><><>
            // <<
            friend std::ostream& operator<<(std::ostream& output, const Matrix& mat);
            // >>
            friend std::istream& operator>>(std::istream& input, Matrix& mat);
            
            //========================================
            // relations between matrixes
            bool operator<(const Matrix& rMat) const;
            bool operator<=(const Matrix& rMat) const;
            bool operator>(const Matrix& rMat) const;
            bool operator>=(const Matrix& rMat) const;
            bool operator==(const Matrix& rMat) const;
            bool operator!=(const Matrix& rMat) const;           
            
        private:    
            vector<double> Vector;
            int Height, Width;
    };
}
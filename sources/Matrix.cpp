#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Matrix.hpp"

using namespace std;
using namespace zich;   

Matrix::Matrix(const vector<double>& values, int height, int width) {
    Vector = values;
    Height = height;
    Width = width;
    if (Vector.size() != Height * Width) {
        throw std::runtime_error("wrong amount of values given");
    }
    if (Height <= 0 || Width <= 0) {
        throw std::runtime_error("matrix cannot be empty");
    }
}
Matrix::~Matrix() {

}

//get functions
int Matrix::get_width() const{
    return Width;
}
int Matrix::get_height() const{
    return Height;
}
double Matrix::get_cell(int x, int y) const{
    if (x >= Width || y >= Height || x < 0 || y < 0) {
        throw std::runtime_error("x or y out of range");
    }
    return Vector[(size_t)(y * Width + x)];
}
vector<double> Matrix::get_vector() const{
    return Vector;
}
//set functions
void Matrix::set_cell(int x, int y, double value) {
    if (x >= Width || y >= Height || x < 0 || y < 0) {
        throw std::runtime_error("x or y out of range");
    }
    Vector[(size_t)(y * Width + x)] = value;
}
void Matrix::set_vector(vector<double> vector) {
    Vector = vector;
}

//+++++++++++++++++++++++++++++++++++++++
// mat + mat
Matrix zich::operator+(Matrix lMat, const Matrix& Matr) {
    lMat += Matr;
    return lMat;
}
// += operator
Matrix& Matrix::operator+=(const Matrix& rMat) {
    same_size(rMat);
    for (size_t i = 0 ; i < Vector.size() ; i++) {
        Vector[i] += rMat.get_vector()[i];
    }
    return *this;
}
// unary + operator
Matrix& Matrix::operator+() {
    return *this;
}
// ++mat
Matrix& Matrix::operator++() {
    for (size_t i = 0 ; i < Vector.size() ; i++) {
        Vector[i]++;
    }
    return *this;
}
// mat++
Matrix Matrix::operator++(int) {
    vector<double> temp = Vector;
    for (size_t i = 0 ; i < Vector.size() ; i++) {
        ++Vector[i];
    }
    Matrix ret{temp, Height, Width};
    return ret;
}

//----------------------------------------
// mat - mat
Matrix zich::operator-(Matrix lMat, const Matrix& rMat) {
    lMat -= rMat;
    return lMat;
}
// -= operator
Matrix& Matrix::operator-=(const Matrix& rMat) {
    (*this) += -rMat;
    return *this;
}
// unary - operator
Matrix zich::operator-(Matrix mat) {
    return mat * (-1);
}
// --mat
Matrix& Matrix::operator--() {
    for (size_t i = 0 ; i < Vector.size() ; i++) {
        Vector[i]--;
    }
    return *this;
}
// mat--
Matrix Matrix::operator--(int) {
    vector<double> temp = Vector;
    for (size_t i = 0 ; i < Vector.size() ; i++) {
        --Vector[i];
    }
    Matrix ret{temp, Height, Width};
    return ret;
}

//****************************************
// mat * mat
Matrix zich::operator*(Matrix lMat, const Matrix& rMat) {
    lMat *= rMat;
    return lMat;
}
// mat * scalar
Matrix zich::operator*(Matrix lMat, double scalar) {
    lMat *= scalar;
    return lMat;
}
Matrix zich::operator*(double scalar,Matrix rMat) {
    rMat *= scalar;
    return rMat;
}
// *=
Matrix& Matrix::operator*=(double scalar) {
    for (size_t i = 0 ; i < Vector.size() ; i++) {
        Vector[i] *= scalar;
    }
    return *this;
}
// *=
Matrix& Matrix::operator*=(const Matrix& rMat) {
    if (get_width() != rMat.get_height()) {
        throw std::runtime_error("the left matrix's width must be equal to the right matrix's height");
    }
    vector<double> vector;
    for (int i = 0 ; i < rMat.get_width() * get_height() ; i++) {
        vector.push_back(0);
    }
    Matrix mat{vector, get_height(), rMat.get_width()};
    for (int i = 0 ; i < get_height() ; i++) {
        for (int j = 0 ; j < rMat.get_width() ; j++) {
            for (int k = 0 ; k < get_width() ; k++) {
                double lCell = get_cell(k, i);
                double rCell = rMat.get_cell(j, k);
                mat.set_cell(j, i, mat.get_cell(j, i) + lCell * rCell);
            }
        }
    }
    *this = mat;
    return *this;
}

//<><><><><><><><><><><><><><><><><><><><>
// <<
ostream& zich::operator<<(ostream& output, const Matrix& mat) {
    string mat_str = "";
    for (int i = 0 ; i < mat.get_height() ; i++) {
        mat_str += "[";
        for (int j = 0 ; j < mat.get_width() ; j++) {
            mat_str += to_string((int)mat.get_cell(j, i));
            if (j < mat.get_width() - 1) {
                mat_str += " ";
            }
        }
        mat_str += "]";
        if (i < mat.get_height() - 1) {
            mat_str += "\n";
        }
    }
    output << mat_str;
    return output;
}
// >>
istream& zich::operator>>(istream& input, Matrix& mat) {
    return input;
}

//========================================
// relations between matrixes
bool Matrix::operator<(const Matrix& rMat) const {
    same_size(rMat);
    double sum1 = 0, sum2 = 0;
    for (size_t i = 0 ; i < Vector.size() ; i++) {
        sum1 += Vector[i];
        sum2 += rMat.get_vector()[i];
    }
    return sum1 < sum2;
}
bool Matrix::operator<=(const Matrix& rMat) const {
    return *this < rMat || *this == rMat;
}
bool Matrix::operator>(const Matrix& rMat) const {
    same_size(rMat);
    double sum1 = 0, sum2 = 0;
    for (size_t i = 0 ; i < Vector.size() ; i++) {
        sum1 += Vector[i];
        sum2 += rMat.get_vector()[i];
    }
    return sum1 > sum2;
}
bool Matrix::operator>=(const Matrix& rMat) const {
    return *this > rMat || *this == rMat;
}
bool Matrix::operator==(const Matrix& rMat) const {
    same_size(rMat);
    for (size_t i = 0 ; i < Vector.size() ; i++) {
        if (Vector[i] != rMat.get_vector()[i]) {
            return false;
        }
    }
    return true;
}
bool Matrix::operator!=(const Matrix& rMat) const {
    return !(*this == rMat);
}

bool Matrix::same_size(Matrix mat) const{
    if (mat.get_height() != Height || mat.get_width() != Width) {
        throw std::runtime_error("cant operate on matrixes of different size");    
    }
    return mat.get_height() == Height && mat.get_width() == Width;
}
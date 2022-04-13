#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <regex>
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
    return Vector[((size_t)y * (size_t)Width + (size_t)x)];
}
vector<double> Matrix::get_vector() const{
    return Vector;
}
//set functions
void Matrix::set_cell(int x, int y, double value) {
    if (x >= Width || y >= Height || x < 0 || y < 0) {
        throw std::runtime_error("x or y out of range");
    }
    Vector[((size_t)y * (size_t)Width + (size_t)x)] = value;
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
Matrix zich::operator-(const Matrix& mat) {
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
    vector<double> vector((size_t)(rMat.get_width() * get_height()), 0);

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
    string mat_str;
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
    // "[1 1 1 1], [1 1 1 1], [1 1 1 1]\n"
    //              |
    //              V
    //         [1, 1, 1, 1]
    //         [1, 1, 1, 1]
    //         [1, 1, 1, 1]
    //  width: 4, height 3
    //  vector: {1,1,1,1,1,1,1,1,1,1,1,1}
    string str_in;
    getline(input, str_in);

    if (regex_match(str_in, regex("(\\[([0-9]+( [0-9]+)*)\\], )*\\[([0-9]+( [0-9]+)*)\\]"))){
        vector<double> vector;
 
        regex words_regex("[0-9]+");
        auto words_begin = sregex_iterator(str_in.begin(), str_in.end(), words_regex);
        auto words_end = sregex_iterator();
    
        int amount = distance(words_begin, words_end);
            
        for (sregex_iterator i = words_begin; i != words_end; ++i) {
            smatch match = *i;                                                 
            string match_str = match.str(); 
            vector.push_back((double)stoi(match_str));
        }
        
        int height = 0;

        for (size_t i = 0; i < str_in.size(); i++) {
            if (str_in[i] == '[') { 
                height++;
            }
        }
        
        int width = 0;
        
        if (height != 0) {
            width = amount/height;
        }
        else {
            throw std::runtime_error("matrix cant be of height 0");
        }

        Matrix temp{vector, width, height};
        mat = temp;
    }
    else {
        throw std::runtime_error("input format is not correct");
    }
    return input;
}

//========================================
// relations between matrixes
bool Matrix::operator<(const Matrix& rMat) const {
    same_size(rMat);
    double sum1 = 0;
    double sum2 = 0;
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
    double sum1 = 0;
    double sum2 = 0;
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

bool Matrix::same_size(const Matrix& mat) const{
    if (mat.get_height() != Height || mat.get_width() != Width) {
        throw std::runtime_error("cant operate on matrixes of different size");    
    }
    return mat.get_height() == Height && mat.get_width() == Width;
}
#include <tensor/tensor.h>
//#include <cstdio>
#include <cmath>
//#include <utility>

#define MAXTWO 10
#define MAXFOUR 10

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

double a2[MAXTWO][MAXTWO];
double b2[MAXTWO][MAXTWO];
double c2[MAXTWO][MAXTWO];
double d2[MAXTWO][MAXTWO];
double a4[MAXFOUR][MAXFOUR][MAXFOUR][MAXFOUR];
double b4[MAXFOUR][MAXFOUR][MAXFOUR][MAXFOUR];
double c4[MAXFOUR][MAXFOUR][MAXFOUR][MAXFOUR];
double d4[MAXFOUR][MAXFOUR][MAXFOUR][MAXFOUR];

using namespace tensor;

enum TestResult {
    kPassed,
    kFailed,
    kException
};

/// Initialize a tensor and a 2-dim matrix with random numbers
void initialize_random(Tensor &tensor, double matrix[MAXTWO][MAXTWO]);
std::pair<double,double> difference(Tensor &tensor, double matrix[MAXTWO][MAXTWO]);

void initialize_random(Tensor &tensor, double matrix[MAXFOUR][MAXFOUR][MAXFOUR][MAXFOUR]);
std::pair<double,double> difference(Tensor &tensor, double matrix[MAXFOUR][MAXFOUR][MAXFOUR][MAXFOUR]);

double test_Cij_plus_equal_Aik_Bkj();
double test_Cij_minus_equal_Aik_Bkj();
double test_Cij_equal_Aik_Bkj();
double test_Cij_equal_Aik_Bjk();
double test_Cij_equal_Aik_Bkj();
double test_Cij_equal_Aik_Bjk();
double test_Cijkl_equal_Aijab_Bklab();
double test_Cikjl_equal_Aijab_Bklab();
double test_Cij_equal_Aiabc_Bjabc();
double test_Cij_equal_Aji();
double test_Cijkl_equal_Akilj();
double test_Cijkl_equal_Akijl();
double test_C_equal_2_A();
double test_C_plus_equal_2_A();
double test_C_minus_equal_2_A();
double test_C_times_equal_2();
double test_C_divide_equal_2();
double test_Cij_equal_Cij();
double test_syev();
double test_Cilkj_equal_Aibaj_Bblak();
double test_Cljik_equal_Abija_Blbak();
double test_Cij_equal_Aij_plus_Bij();
double test_Dij_equal_Aij_plus_Bij_plus_Cij();
double test_Cij_equal_Aij_minus_Bij();
double test_Dij_equal_Aij_minus_Bij_plus_Cij();

std::tuple<std::string,TestResult,double> test_C_equal_A_B(std::string c_ind,std::string a_ind,std::string b_ind,
                                               std::vector<int> c_dim,std::vector<int> a_dim,std::vector<int> b_dim);

double zero = 1.0e-12;

int main(int argc, char* argv[])
{
    srand (time(NULL));
    tensor::initialize(argc, argv);

    auto test_functions = {
            std::make_pair(test_C_equal_2_A,               "C(\"ij\") = 2.0 * A(\"ij\")"),
            std::make_pair(test_C_plus_equal_2_A,          "C(\"ij\") += 2.0 * A(\"ij\")"),
            std::make_pair(test_C_minus_equal_2_A,         "C(\"ij\") -= 2.0 * A(\"ij\")"),
            std::make_pair(test_C_times_equal_2,           "C(\"ij\") *= 2.0"),
            std::make_pair(test_C_divide_equal_2,          "C(\"ij\") /= 2.0"),
            std::make_pair(test_Cij_equal_Aik_Bkj,         "C(\"ij\") = A(\"ik\") * B(\"kj\")"),
            std::make_pair(test_Cij_equal_Aik_Bjk,         "C(\"ij\") = A(\"ik\") * B(\"jk\")"),
            std::make_pair(test_Cij_plus_equal_Aik_Bkj,    "C(\"ij\") += A(\"ik\") * B(\"kj\")"),
            std::make_pair(test_Cij_minus_equal_Aik_Bkj,   "C(\"ij\") -= A(\"ik\") * B(\"kj\")"),
            std::make_pair(test_Cijkl_equal_Aijab_Bklab,   "C(\"ijkl\") += A(\"ijab\") * B(\"klab\")"),
            std::make_pair(test_Cij_equal_Aiabc_Bjabc,     "C(\"ij\") += A(\"iabc\") * B(\"jabc\")"),
            std::make_pair(test_Cikjl_equal_Aijab_Bklab,   "C(\"ikjl\") += A(\"ijab\") * B(\"klab\")"),
            std::make_pair(test_Cij_equal_Aji,             "C(\"ij\") = A(\"ji\")"),
            std::make_pair(test_Cijkl_equal_Akilj,         "C(\"ijkl\") = A(\"kilj\")"),
            std::make_pair(test_Cijkl_equal_Akijl,         "C(\"ijkl\") = A(\"kijl\")"),
            std::make_pair(test_Cij_equal_Cij,             "C(\"ij\") = C(\"ji\") not allowed"),
            std::make_pair(test_Cilkj_equal_Aibaj_Bblak,   "C(\"ilkj\") += A(\"ibaj\") * B(\"blak\")"),
            std::make_pair(test_Cljik_equal_Abija_Blbak,   "C(\"ljik\") += A(\"bija\") * B(\"lbak\")"),
            std::make_pair(test_Cij_equal_Aij_plus_Bij,    "C(\"ij\") = A(\"ij\") + B(\"ij\")"),
            std::make_pair(test_Dij_equal_Aij_plus_Bij_plus_Cij, "D(\"ij\") = A(\"ij\") + B(\"ij\") + C(\"ij\")"),
            std::make_pair(test_Cij_equal_Aij_minus_Bij,    "C(\"ij\") = A(\"ij\") - 5.0 * B(\"ij\")"),
            std::make_pair(test_Dij_equal_Aij_minus_Bij_plus_Cij, "D(\"ij\") = A(\"ij\") - B(\"ij\") + 2.0 * C(\"ij\")"),
            std::make_pair(test_syev,                      "Diagonalization (not confirmed)")
    };

    std::vector<std::tuple<std::string,TestResult,double>> results;

    for (auto test_function : test_functions) {
        printf("  Testing %s\n", test_function.second);
        try {
            double result = test_function.first();
            results.push_back(std::make_tuple(test_function.second,
                                              std::fabs(result) < zero ? kPassed : kFailed,
                                              result));
        }
        catch (std::exception& e) {
            printf("    Exception caught: %s\n", e.what());
            results.push_back(std::make_tuple(test_function.second,
                                              kException,
                                              0.0));
        }
    }

    results.push_back(test_C_equal_A_B("ij","ik","jk",{0,1},{0,2},{1,2}));
    results.push_back(test_C_equal_A_B("ij","ik","kj",{0,1},{0,2},{2,1}));
    results.push_back(test_C_equal_A_B("ij","ki","jk",{0,1},{2,0},{1,2}));
    results.push_back(test_C_equal_A_B("ij","ki","kj",{0,1},{2,0},{2,1}));
    results.push_back(test_C_equal_A_B("ji","ik","jk",{1,0},{0,2},{1,2}));
    results.push_back(test_C_equal_A_B("ji","ik","kj",{1,0},{0,2},{2,1}));
    results.push_back(test_C_equal_A_B("ji","ki","jk",{1,0},{2,0},{1,2}));
    results.push_back(test_C_equal_A_B("ji","ki","kj",{1,0},{2,0},{2,1}));

    tensor::finalize();

    bool success = true;
    for (auto sb : results){
        if (std::get<1>(sb) != kPassed) success = false;
    }

    if(true){
        printf("\n\n Summary of tests:");

        printf("\n %-50s %12s %s","Test","Max. error","Result");
        printf("\n %s",std::string(73,'-').c_str());
        for (auto sb : results){
            printf("\n %-50s %7e %s",
                   std::get<0>(sb).c_str(),
                   std::get<2>(sb),
                   std::get<1>(sb) == kPassed ? ANSI_COLOR_GREEN "Passed" ANSI_COLOR_RESET : std::get<1>(sb) == kFailed ? ANSI_COLOR_RED "Failed" ANSI_COLOR_RESET : ANSI_COLOR_YELLOW "Exception" ANSI_COLOR_RESET);
        }
        printf("\n %s",std::string(73,'-').c_str());
        printf("\n Tests: %s\n",success ? "All passed" : "Some failed");
    }

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

Tensor build_and_fill(const std::string& name, const Dimension& dims, double matrix[MAXTWO][MAXTWO])
{
    Tensor T = Tensor::build(kCore, name, dims);
    initialize_random(T, matrix);
    std::pair<double,double> a_diff = difference(T, matrix);
    if (std::fabs(a_diff.second) > zero) throw std::runtime_error("Tensor and standard matrix don't match.");
    return T;
}

Tensor build_and_fill(const std::string& name, const Dimension& dims, double matrix[MAXFOUR][MAXFOUR][MAXFOUR][MAXFOUR])
{
    Tensor T = Tensor::build(kCore, name, dims);
    initialize_random(T, matrix);
    std::pair<double,double> a_diff = difference(T, matrix);
    if (std::fabs(a_diff.second) > zero) throw std::runtime_error("Tensor and standard matrix don't match.");
    return T;
}

void initialize_random(Tensor &tensor, double matrix[MAXTWO][MAXTWO])
{
    size_t n0 = tensor.dims()[0];
    size_t n1 = tensor.dims()[1];
    double* vec = new double[n0 * n1];
    for (size_t i = 0, ij = 0; i < n0; ++i){
        for (size_t j = 0; j < n1; ++j, ++ij){
            double randnum = double(std::rand())/double(RAND_MAX);
            matrix[i][j] = randnum;
            vec[ij] = randnum;
        }
    }
    tensor.set_data(vec);
    delete[] vec;
}

std::pair<double,double> difference(Tensor &tensor, double matrix[MAXTWO][MAXTWO])
{
    size_t n0 = tensor.dims()[0];
    size_t n1 = tensor.dims()[1];

    size_t numel = tensor.numel();
    double* result = new double[numel];

    tensor.get_data(result);

    double sum_diff = 0.0;
    double max_diff = 0.0;
    for (size_t i = 0, ij = 0; i < n0; ++i){
        for (size_t j = 0; j < n1; ++j, ++ij){
            double diff = std::fabs(matrix[i][j] - result[ij]);
            sum_diff += diff;
            max_diff = std::max(diff,max_diff);
        }
    }
    delete[] result;
    return std::make_pair(sum_diff,max_diff);
}

void initialize_random(Tensor &tensor, double matrix[MAXFOUR][MAXFOUR][MAXFOUR][MAXFOUR])
{
    size_t n0 = tensor.dims()[0];
    size_t n1 = tensor.dims()[1];
    size_t n2 = tensor.dims()[2];
    size_t n3 = tensor.dims()[3];

    double* vec = new double[n0 * n1 * n2 * n3];
    for (size_t i = 0, ijkl = 0; i < n0; ++i){
        for (size_t j = 0; j < n1; ++j){
            for (size_t k = 0; k < n2; ++k){
                for (size_t l = 0; l < n3; ++l, ++ijkl){
                    double randnum = double(std::rand())/double(RAND_MAX);
                    matrix[i][j][k][l] = randnum;
                    vec[ijkl] = randnum;
                }
            }
        }
    }
    tensor.set_data(vec);
    delete[] vec;
}

std::pair<double,double> difference(Tensor &tensor, double matrix[MAXFOUR][MAXFOUR][MAXFOUR][MAXFOUR])
{
    size_t n0 = tensor.dims()[0];
    size_t n1 = tensor.dims()[1];
    size_t n2 = tensor.dims()[2];
    size_t n3 = tensor.dims()[3];

    size_t numel = tensor.numel();

    double* result = new double[numel];

    tensor.get_data(result);

    double sum_diff = 0.0;
    double max_diff = 0.0;

    for (size_t i = 0, ijkl = 0; i < n0; ++i){
        for (size_t j = 0; j < n1; ++j){
            for (size_t k = 0; k < n2; ++k){
                for (size_t l = 0; l < n3; ++l, ++ijkl){
                    double diff = std::fabs(matrix[i][j][k][l] - result[ijkl]);
                    sum_diff += diff;
                    max_diff = std::max(diff,max_diff);
                }
            }
        }
    }
    delete[] result;
    return std::make_pair(sum_diff,max_diff);
}

std::tuple<std::string,TestResult,double> test_C_equal_A_B(std::string c_ind,std::string a_ind,std::string b_ind,
                                                           std::vector<int> c_dim,std::vector<int> a_dim,std::vector<int> b_dim)
{
    std::string test = "C(\"" + c_ind + "\") += A(\"" + a_ind + "\") * B(\"" + b_ind + "\")";
    printf("  Testing %s\n",test.c_str());

    std::vector<size_t> dims;
    dims.push_back(9);
    dims.push_back(6);
    dims.push_back(7);

    size_t ni = 9;
    size_t nj = 6;
    size_t nk = 7;

    Tensor A = build_and_fill("A", {dims[a_dim[0]], dims[a_dim[1]]}, a2);
    Tensor B = build_and_fill("B", {dims[b_dim[0]], dims[b_dim[1]]}, b2);
    Tensor C = build_and_fill("C", {dims[c_dim[0]], dims[c_dim[1]]}, c2);

    C(c_ind) += A(a_ind) * B(b_ind);

    std::vector<size_t> n(3);
    for (n[0] = 0; n[0] < ni; ++n[0]){
        for (n[1] = 0; n[1] < nj; ++n[1]){
            for (n[2] = 0; n[2] < nk; ++n[2]){
                int aind1 = n[a_dim[0]];
                int aind2 = n[a_dim[1]];
                int bind1 = n[b_dim[0]];
                int bind2 = n[b_dim[1]];
                int cind1 = n[c_dim[0]];
                int cind2 = n[c_dim[1]];
                c2[cind1][cind2] += a2[aind1][aind2] * b2[bind1][bind2];
            }
        }
    }
    std::pair<double,double>c_diff = difference(C, c2);

    TestResult tr = std::fabs(c_diff.second) < zero ? kPassed : kFailed;

    return std::make_tuple(test,tr,c_diff.second);
}

double test_Cij_plus_equal_Aik_Bkj()
{
    size_t ni = 9;
    size_t nj = 6;
    size_t nk = 7;

    Tensor A = build_and_fill("A", {ni, nk}, a2);
    Tensor B = build_and_fill("B", {nk, nj}, b2);
    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C("ij") += A("ik") * B("kj");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            for (size_t k = 0; k < nk; ++k){
                c2[i][j] += a2[i][k] * b2[k][j];
            }
        }
    }

    return difference(C, c2).second;
}


double test_Cij_minus_equal_Aik_Bkj()
{
    size_t ni = 9;
    size_t nj = 6;
    size_t nk = 7;

    Tensor A = build_and_fill("A", {ni, nk}, a2);
    Tensor B = build_and_fill("B", {nk, nj}, b2);
    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C("ij") -= A("ik") * B("kj");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            for (size_t k = 0; k < nk; ++k){
                c2[i][j] -= a2[i][k] * b2[k][j];
            }
        }
    }

    return difference(C, c2).second;
}

double test_Cij_equal_Aik_Bkj()
{
    size_t ni = 9;
    size_t nj = 6;
    size_t nk = 7;

    Tensor A = build_and_fill("A", {ni, nk}, a2);
    Tensor B = build_and_fill("B", {nk, nj}, b2);
    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C.zero();
    C("ij") = A("ik") * B("kj");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            c2[i][j] = 0.0;
            for (size_t k = 0; k < nk; ++k){
                c2[i][j] += a2[i][k] * b2[k][j];
            }
        }
    }

    return difference(C, c2).second;
}

double test_Cij_equal_Aik_Bjk()
{
    size_t ni = 9;
    size_t nj = 6;
    size_t nk = 7;

    Tensor A = build_and_fill("A", {ni, nk}, a2);
    Tensor B = build_and_fill("B", {nj, nk}, b2);
    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C("ij") = A("ik") * B("jk");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            c2[i][j] = 0.0;
            for (size_t k = 0; k < nk; ++k){
                c2[i][j] += a2[i][k] * b2[j][k];
            }
        }
    }

    return difference(C, c2).second;
}


double test_Cijkl_equal_Aijab_Bklab()
{
    size_t ni = 9;
    size_t nj = 6;
    size_t nk = 7;
    size_t nl = 9;
    size_t na = 6;
    size_t nb = 7;

    Tensor A = build_and_fill("A", {ni, nj, na, nb}, a4);
    Tensor B = build_and_fill("B", {nk, nl, na, nb}, b4);
    Tensor C = build_and_fill("C", {ni, nj, nk, nl}, c4);

    C("ijkl") += A("ijab") * B("klab");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            for (size_t k = 0; k < nk; ++k){
                for (size_t l = 0; l < nl; ++l){
                    for (size_t a = 0; a < na; ++a){
                        for (size_t b = 0; b < nb; ++b){
                            c4[i][j][k][l] += a4[i][j][a][b] * b4[k][l][a][b];
                        }
                    }
                }
            }
        }
    }

    return difference(C, c4).second;
}


double test_Cikjl_equal_Aijab_Bklab()
{
    size_t ni = 9;
    size_t nj = 6;
    size_t nk = 7;
    size_t nl = 9;
    size_t na = 6;
    size_t nb = 7;

    Tensor A = build_and_fill("A", {ni, nj, na, nb}, a4);
    Tensor B = build_and_fill("B", {nk, nl, na, nb}, b4);
    Tensor C = build_and_fill("C", {ni, nk, nj, nl}, c4);

    C("ikjl") += A("ijab") * B("klab");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            for (size_t k = 0; k < nk; ++k){
                for (size_t l = 0; l < nl; ++l){
                    for (size_t a = 0; a < na; ++a){
                        for (size_t b = 0; b < nb; ++b){
                            c4[i][k][j][l] += a4[i][j][a][b] * b4[k][l][a][b];
                        }
                    }
                }
            }
        }
    }

    return difference(C, c4).second;
}

double test_Cij_equal_Aiabc_Bjabc()
{
    size_t ni = 9;
    size_t nj = 6;
    size_t na = 6;
    size_t nb = 7;
    size_t nc = 8;

    Tensor A = build_and_fill("A", {ni, na, nb, nc}, a4);
    Tensor B = build_and_fill("B", {nj, na, nb, nc}, b4);
    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C("ij") += A("iabc") * B("jabc");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            for (size_t a = 0; a < na; ++a){
                for (size_t b = 0; b < nb; ++b){
                    for (size_t c = 0; c < nc; ++c){
                        c2[i][j] += a4[i][a][b][c] * b4[j][a][b][c];
                    }
                }
            }
        }
    }

    return difference(C, c2).second;
}

double test_C_equal_2_A()
{
    size_t ni = 9;
    size_t nj = 6;

    Tensor A = build_and_fill("A", {ni, nj}, a2);
    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C("ij") = 2.0 * A("ij");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            c2[i][j] = 2.0 * a2[i][j];
        }
    }

    return difference(C, c2).second;
}

double test_C_plus_equal_2_A()
{
    size_t ni = 9;
    size_t nj = 6;

    Tensor A = build_and_fill("A", {ni, nj}, a2);
    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C("ij") += 2.0 * A("ij");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            c2[i][j] += 2.0 * a2[i][j];
        }
    }

    return difference(C, c2).second;
}

double test_C_minus_equal_2_A()
{
    size_t ni = 9;
    size_t nj = 6;

    Tensor A = build_and_fill("A", {ni, nj}, a2);
    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C("ij") -= 2.0 * A("ij");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            c2[i][j] -= 2.0 * a2[i][j];
        }
    }

    return difference(C, c2).second;
}

double test_C_times_equal_2()
{
    size_t ni = 9;
    size_t nj = 6;

    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C("ij") *= 2.0;

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            c2[i][j] *= 2.0;
        }
    }

    return difference(C, c2).second;
}

double test_C_divide_equal_2()
{
    size_t ni = 9;
    size_t nj = 6;

    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C("ij") /= 2.0;

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            c2[i][j] /= 2.0;
        }
    }

    return difference(C, c2).second;
}

double test_Cij_equal_Aji()
{
    size_t ni = 9;
    size_t nj = 6;

    Tensor A = build_and_fill("A", {nj, ni}, a2);
    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C("ij") = A("ji");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            c2[i][j] = a2[j][i];
        }
    }

    return difference(C, c2).second;
}

double test_Cijkl_equal_Akijl()
{
    size_t ni = 9;
    size_t nj = 6;
    size_t nk = 5;
    size_t nl = 4;

    Tensor A = build_and_fill("A", {nk, ni, nj, nl}, a4);
    Tensor C = build_and_fill("C", {ni, nj, nk, nl}, c4);

    C("ijkl") = A("kijl");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            for (size_t k = 0; k < nk; ++k){
                for (size_t l = 0; l < nl; ++l){
                    c4[i][j][k][l] = a4[k][i][j][l];
                }
            }
        }
    }

    return difference(C, c4).second;
}

double test_Cijkl_equal_Akilj()
{
    size_t ni = 9;
    size_t nj = 6;
    size_t nk = 5;
    size_t nl = 4;

    Tensor A = build_and_fill("A", {nk, ni, nl, nj}, a4);
    Tensor C = build_and_fill("C", {ni, nj, nk, nl}, c4);

    C("ijkl") = A("kilj");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            for (size_t k = 0; k < nk; ++k){
                for (size_t l = 0; l < nl; ++l){
                    c4[i][j][k][l] = a4[k][i][l][j];
                }
            }
        }
    }

    return difference(C, c4).second;
}

double test_Cij_equal_Cij()
{
    size_t ni = 9;
    size_t nj = 6;

    Tensor C = build_and_fill("C", {ni, nj}, c2);

    try {
        C("ij") = C("ij");
    }
    catch (std::exception& e) {
        return 0.00;
    }
    return 1.0;
}

double test_syev()
{
    size_t ni = 9;

    Tensor C = build_and_fill("C", {ni, ni}, c2);

    auto result = C.syev(kDescending);

//    Tensor vectors = result["eigenvectors"];

//    result["eigenvectors"].print(stdout, 1);
//    result["eigenvalues"].print(stdout, 1);

    return 0.0;
}

double test_Cilkj_equal_Aibaj_Bblak()
{
    size_t ni = 9;
    size_t nj = 6;
    size_t nk = 7;
    size_t nl = 9;
    size_t na = 6;
    size_t nb = 7;

    Tensor A = build_and_fill("A", {ni, nb, na, nj}, a4);
    Tensor B = build_and_fill("B", {nb, nl, na, nk}, b4);
    Tensor C = build_and_fill("C", {ni, nl, nk, nj}, c4);

    C("ilkj") += A("ibaj") * B("blak");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            for (size_t k = 0; k < nk; ++k){
                for (size_t l = 0; l < nl; ++l){
                    for (size_t a = 0; a < na; ++a){
                        for (size_t b = 0; b < nb; ++b){
                            c4[i][l][k][j] += a4[i][b][a][j] * b4[b][l][a][k];
                        }
                    }
                }
            }
        }
    }

    return difference(C, c4).second;
}

double test_Cljik_equal_Abija_Blbak()
{
    size_t ni = 9;
    size_t nj = 6;
    size_t nk = 7;
    size_t nl = 9;
    size_t na = 6;
    size_t nb = 7;

    Tensor A = build_and_fill("A", {nb, ni, nj, na}, a4);
    Tensor B = build_and_fill("B", {nl, nb, na, nk}, b4);
    Tensor C = build_and_fill("C", {nl, nj, ni, nk}, c4);

    C("ljik") += A("bija") * B("lbak");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            for (size_t k = 0; k < nk; ++k){
                for (size_t l = 0; l < nl; ++l){
                    for (size_t a = 0; a < na; ++a){
                        for (size_t b = 0; b < nb; ++b){
                            c4[l][j][i][k] += a4[b][i][j][a] * b4[l][b][a][k];
                        }
                    }
                }
            }
        }
    }

    return difference(C, c4).second;
}

double test_Cij_equal_Aij_plus_Bij()
{
    size_t ni = 9;
    size_t nj = 6;

    Tensor A = build_and_fill("A", {ni, nj}, a2);
    Tensor B = build_and_fill("B", {ni, nj}, b2);
    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C("ij") = A("ij") + B("ij");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            c2[i][j] = a2[i][j] + b2[i][j];
        }
    }

    return difference(C, c2).second;
}

double test_Dij_equal_Aij_plus_Bij_plus_Cij()
{
    size_t ni = 9, nj = 6;

    Dimension dims = {ni, nj};
    Tensor A = build_and_fill("A", dims, a2);
    Tensor B = build_and_fill("B", dims, b2);
    Tensor C = build_and_fill("C", dims, c2);
    Tensor D = build_and_fill("D", dims, d2);

    D("ij") = A("ij") + B("ij") + C("ij");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            d2[i][j] = a2[i][j] + b2[i][j] + c2[i][j];
        }
    }

    return difference(D, d2).second;
}

double test_Cij_equal_Aij_minus_Bij()
{
    size_t ni = 9;
    size_t nj = 6;

    Tensor A = build_and_fill("A", {ni, nj}, a2);
    Tensor B = build_and_fill("B", {ni, nj}, b2);
    Tensor C = build_and_fill("C", {ni, nj}, c2);

    C("ij") = A("ij") - 5.0 * B("ij");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            c2[i][j] = a2[i][j] - 5.0 * b2[i][j];
        }
    }

    return difference(C, c2).second;
}

double test_Dij_equal_Aij_minus_Bij_plus_Cij()
{
    size_t ni = 9, nj = 6;

    Dimension dims = {ni, nj};
    Tensor A = build_and_fill("A", dims, a2);
    Tensor B = build_and_fill("B", dims, b2);
    Tensor C = build_and_fill("C", dims, c2);
    Tensor D = build_and_fill("D", dims, d2);

    D("ij") = A("ij") - B("ij") + 2.0 * C("ij");

    for (size_t i = 0; i < ni; ++i){
        for (size_t j = 0; j < nj; ++j){
            d2[i][j] = a2[i][j] - b2[i][j] + 2.0 * c2[i][j];
        }
    }

    return difference(D, d2).second;
}

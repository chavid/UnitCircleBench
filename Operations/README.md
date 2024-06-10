
# Comparatif -O0

if, divide/virtual |      saxpy |     divide |    virtual | divide+virtual |
:----------------- | ---------: | ---------: | ---------: | -------------: |
no                 |       4.2  |       4.4  |       4.2  |           4.3  |
if                 |       5.2  |       5.6  |       5.3  |           5.7  |
bool               |       5.7  |       6.1  |       5.6  |           6.0  |

# Comparatif -O2

if, divide/virtual |      saxpy |     divide |    virtual | divide+virtual |
:----------------- | ---------: | ---------: | ---------: | -------------: |
no                 |       0.30 |       0.32 |       0.36 |           0.37 |
if                 |       0.86 |       1.00 |       0.82 |           0.97 |
bool               |       0.69 |       0.71 |       0.65 |           0.75 |


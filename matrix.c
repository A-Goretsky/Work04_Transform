#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"


/*======== struct matrix * make_translate() ==========
Inputs:  int x
         int y
         int z
Returns: The translation matrix created using x, y and z
as the translation offsets.
====================*/
struct matrix * make_translate(double x, double y, double z) {
    struct matrix *translate;
    translate = new_matrix(4, 4);
    ident(translate);
    translate->m[0][3] = x;
    translate->m[1][3] = y;
    translate->m[2][3] = z;
    return translate;
}

/*======== struct matrix * make_scale() ==========
Inputs:  int x
         int y
         int z
Returns: The translation matrix creates using x, y and z
as the scale factors
====================*/
struct matrix * make_scale(double x, double y, double z) {
    struct matrix *scale;
    scale = new_matrix(4, 4);
    ident(scale);
    scale->m[0][0] = x;
    scale->m[1][1] = y;
    scale->m[2][2] = z;
    return scale;
}

/*======== struct matrix * make_rotX() ==========
Inputs:  double theta

Returns: The rotation matrix created using theta as the
angle of rotation and X as the axis of rotation.
====================*/
struct matrix * make_rotX(double theta) {
    struct matrix *rotx;
    rotx = new_matrix(4, 4);
    ident(rotx);
    theta = (M_PI / 180.0) * theta;
    rotx->m[1][1] = cos(theta);
    rotx->m[1][2] = -1 * sin(theta);
    rotx->m[2][1] = sin(theta);
    rotx->m[2][2] = cos(theta);
    return rotx;
}

/*======== struct matrix * make_rotY() ==========
Inputs:  double theta
         char c
Returns: The rotation matrix created using theta as the
angle of rotation and Y as the axis of rotation.
====================*/
struct matrix * make_rotY(double theta) {
    struct matrix *roty;
    roty = new_matrix(4, 4);
    ident(roty);
    theta = (M_PI / 180.0) * theta;
    roty->m[0][0] = cos(theta);
    roty->m[0][2] = sin(theta);
    roty->m[2][0] = -1 * sin(theta);
    roty->m[2][2] = cos(theta);
    return roty;
}

/*======== struct matrix * make_rotZ() ==========
Inputs:  double theta
         char c
Returns: The rotation matrix created using theta as the
angle of rotation and Z as the axis of rotation.
====================*/
struct matrix * make_rotZ(double theta) {
    struct matrix *rotz;
    rotz = new_matrix(4, 4);
    ident(rotz);
    theta = (M_PI / 180.0) * theta;
    rotz->m[0][0] = cos(theta);
    rotz->m[0][1] = -1 * sin(theta);
    rotz->m[1][0] = sin(theta);
    rotz->m[1][1] = cos(theta);
    return rotz;
}


/*-------------- void print_matrix() --------------
Inputs:  struct matrix *m
Returns:

print the matrix
*/
void print_matrix(struct matrix *m) {
    int x;
    int y;
    for (x = 0; x < m->rows; x++) {
        printf("|");
        for (y = 0; y < m->cols; y++) {
            double temp = m->m[x][y];
            printf(" %.2lf ", temp);
        }
        printf("|\n");
    }
}

/*void print_matrix(struct matrix *m) {

  int r, c;
  for (r=0; r < m->rows; r++) {
    for (c=0; c < m->lastcol; c++)
      printf("%0.2f ", m->m[r][c]);
    printf("\n");
  }
}//end print_matrix
*/
/*-------------- void ident() --------------
Inputs:  struct matrix *m <-- assumes m is a square matrix
Returns:

turns m in to an identity matrix
*/
void ident(struct matrix *m) {
  int r, c;

  for (r=0; r < m->rows; r++)
    for (c=0; c < m->cols; c++)
      if ( r == c )
	m->m[r][c] = 1;
      else
	m->m[r][c] = 0;
  m->lastcol = m->cols;
}//end ident


/*-------------- void scalar_mult() --------------
Inputs:  double x
         struct matrix *m
Returns:

multiply each element of m by x
*/
void scalar_mult(double x, struct matrix *m) {
  int r, c;
  for (r=0; r < m->rows; r++)
    for (c=0; c < m->lastcol; c++)
      m->m[r][c] *= x;
}//end scalar_mult


/*-------------- void matrix_mult() --------------
Inputs:  struct matrix *a
         struct matrix *b
Returns:

a*b -> b
*/
void matrix_mult(struct matrix *a, struct matrix *b) {
  int r, c;
  struct matrix *tmp;
  tmp = new_matrix(4, 1);

  for (c=0; c < b->lastcol; c++) {

    //copy current col (point) to tmp
    for (r=0; r < b->rows; r++)
      tmp->m[r][0] = b->m[r][c];

    for (r=0; r < b->rows; r++)
      b->m[r][c] = a->m[r][0] * tmp->m[0][0] +
	a->m[r][1] * tmp->m[1][0] +
	a->m[r][2] * tmp->m[2][0] +
	a->m[r][3] * tmp->m[3][0];
  }
  free_matrix(tmp);
}//end matrix_mult


/*===============================================
  These Functions do not need to be modified
  ===============================================*/

/*-------------- struct matrix *new_matrix() --------------
Inputs:  int rows
         int cols
Returns:

Once allocated, access the matrix as follows:
m->m[r][c]=something;
if (m->lastcol)...
*/
struct matrix *new_matrix(int rows, int cols) {
  double **tmp;
  int i;
  struct matrix *m;

  tmp = (double **)malloc(rows * sizeof(double *));
  for (i=0;i<rows;i++) {
      tmp[i]=(double *)malloc(cols * sizeof(double));
    }

  m=(struct matrix *)malloc(sizeof(struct matrix));
  m->m=tmp;
  m->rows = rows;
  m->cols = cols;
  m->lastcol = 0;

  return m;
}


/*-------------- void free_matrix() --------------
Inputs:  struct matrix *m
Returns:

1. free individual rows
2. free array holding row pointers
3. free actual matrix
*/
void free_matrix(struct matrix *m) {

  int i;
  for (i=0;i<m->rows;i++) {
      free(m->m[i]);
    }
  free(m->m);
  free(m);
}


/*======== void grow_matrix() ==========
Inputs:  struct matrix *m
         int newcols
Returns:

Reallocates the memory for m->m such that it now has
newcols number of collumns
====================*/
void grow_matrix(struct matrix *m, int newcols) {

  int i;
  for (i=0;i<m->rows;i++) {
      m->m[i] = realloc(m->m[i],newcols*sizeof(double));
  }
  m->cols = newcols;
}


/*-------------- void copy_matrix() --------------
Inputs:  struct matrix *a
         struct matrix *b
Returns:

copy matrix a to matrix b
*/
void copy_matrix(struct matrix *a, struct matrix *b) {

  int r, c;

  for (r=0; r < a->rows; r++)
    for (c=0; c < a->cols; c++)
      b->m[r][c] = a->m[r][c];
}

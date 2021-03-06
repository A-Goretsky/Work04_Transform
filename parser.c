#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename
          struct matrix * transform,
          struct matrix * pm,
          screen s
Returns:

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix -
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 ident: set the transform matrix to the identity matrix -
	 scale: create a scale matrix,
	    then multiply the transform matrix by the scale matrix -
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix,
	    then multiply the transform matrix by the translation matrix -
	    takes 3 arguments (tx, ty, tz)
	 rotate: create an rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 2 arguments (axis, theta) axis should be x y or z
	 apply: apply the current transformation matrix to the
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename,
                  struct matrix * transform,
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);
  color c;
  c.red = 0;
  c.green = 255;
  c.blue = 255;

  if ( strcmp(filename, "stdin") == 0 )
    f = stdin;
  else
    f = fopen(filename, "r");

  double xvals[4];
  double yvals[4];
  double zvals[4];
  struct matrix *temp;
  char rotate_axis;
  double rotate_deg;

  while ( fgets(line, sizeof(line), f) != NULL ) {
    //Place a terminating null
    line[strlen(line)-1]='\0';

    //Run line command.
    if (strncmp(line, "line", strlen(line)) == 0) {
        fgets(line, sizeof(line), f);
        sscanf(line, "%lf %lf %lf %lf %lf %lf",
                      xvals, yvals, zvals, xvals + 1, yvals + 1, zvals + 1);
        add_edge(edges, xvals[0], yvals[0], zvals[0], xvals[1], yvals[1], zvals[1]);
        //PARSE NEXT LINE
        //add_edge("ARGUMENTS RESULTING FROM PARSE")
    }

    //Run Ident command.
    else if (strncmp(line, "ident", strlen(line)) == 0) {
        ident(transform);
    }

    //Run Translation
    else if (strncmp(line, "move", strlen(line)) == 0) {
        fgets(line, sizeof(line), f);
        sscanf(line, "%lf %lf %lf",
                      xvals, yvals, zvals);
        temp = make_translate(xvals[0], yvals[0], zvals[0]);
        matrix_mult(temp, transform);
        free_matrix(temp);
    }

    //Run Scaling
    else if (strncmp(line, "scale", strlen(line)) == 0) {
        fgets(line, sizeof(line), f);
        sscanf(line, "%lf %lf %lf",
                      xvals, yvals, zvals);
        temp = make_scale(xvals[0], yvals[0], zvals[0]);
        matrix_mult(temp, transform);
        free_matrix(temp);
    }

    //Run rotation
    else if (strncmp(line, "rotate", strlen(line)) == 0) {
        fgets(line, sizeof(line), f);
        sscanf(line, "%c %lf",
                      &rotate_axis, &rotate_deg);
        //rotate_deg = (M_PI / 180) * rotate_deg;
        if (rotate_axis == 'x') {
            temp = make_rotX(rotate_deg);
        }
        else if (rotate_axis == 'y') {
            temp = make_rotY(rotate_deg);
        }
        else if (rotate_axis == 'z') {
            temp = make_rotZ(rotate_deg);
        }
        matrix_mult(temp, transform);
        //PARSE NEXT LINE
        //Check 1st Argument for which rotation matrix.
        //Make Rotation Matrix
        //Multiply Matrices
        free_matrix(temp);
    }

    //Run Matrix Application
    else if (strncmp(line, "apply", strlen(line)) == 0) {
        matrix_mult(transform, edges);
    }

    //Run Display
    else if (strncmp(line, "display", strlen(line)) == 0) {
        clear_screen(s);
        draw_lines(edges, s, c);
        display(s);
    }

    //Run Save
    else if (strncmp(line, "save", strlen(line)) == 0) {
        fgets(line, sizeof(line), f);
        *strchr(line, '\n') = 0;
        clear_screen(s);
        draw_lines(edges, s, c);
        save_extension(s, line);
    }

  }
}

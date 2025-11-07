/* This is the header file defining all global variables for the simulator */
#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>

/* Basic data structures */
struct tdata {
    int num_triangles;
    int *triangles;
    float *areas;
};

struct pdata {
    int num_points;
    float *coords;
    int *types;
};

struct ndata {
    int num_nodes;
    int *node_nums;
    float *values;
};

struct rdata {
    int num_regions;
    int *region_nums;
    float *region_data;
};

struct edata {
    int num_edges;
    int *edge_nums;
    float *edge_data;
};

struct sreg {
    int start;
    int end;
    float value;
};

/* Global variables - all defined in globals.c */
extern struct tdata *tri;        /* Triangle data */
extern struct pdata *pt;         /* Point data */
extern struct ndata *nd;         /* Node data */
extern struct rdata *reg;        /* Region data */
extern struct edata *edg;        /* Edge data */

extern int ne;                  /* Number of elements */
extern int nn;                  /* Number of nodes */
extern int np;                  /* Number of points */
extern int nreg;               /* Number of regions */
extern int nvrt;               /* Number of vertices */
extern int ned;                /* Number of edges */
extern int nedg;               /* Number of geometric edges */

extern int mode;               /* Global mode setting */
extern int verbose;            /* Verbosity level */
extern int oxide_model;        /* Which oxide model to use */
extern int oxide_grid;         /* Type of oxide grid */
extern float oxide_Ltol;       /* Length tolerance for oxide model */
extern float oxide_Etol;       /* Energy tolerance for oxide model */
extern int gloop_imin;        /* Grid loop parameters */
extern int gloop_emin;
extern int gloop_emax;
extern FILE *cpufile;         /* CPU time log file */
extern char *MANDIR;          /* Manual directory */

/* Solution mapping */
extern int *soltoimp;         /* Solution to impurity mapping */
extern int *imptosol;         /* Impurity to solution mapping */
extern int n_imp;             /* Number of impurities */
extern int process_step;      /* Current process step */
extern int total;             /* Total number of process steps */
extern int nsreg;             /* Number of solution regions */
extern struct sreg *sreg;     /* Solution region data */

/* Global flags */
extern int nd2tri_dirty;      /* Node to triangle list needs rebuild */
extern int nd2edg_dirty;      /* Node to edge list needs rebuild */
extern int bc_dirty;          /* Boundary condition data needs rebuild */
extern int edge_dirty;        /* Edge data needs rebuild */ 
extern int neigh_dirty;       /* Neighbor data needs rebuild */
extern int clkws_dirty;       /* Clockwise ordering needs rebuild */
extern int geom_dirty;        /* Geometry data needs rebuild */
extern int reg_dirty;         /* Region data needs rebuild */
extern int need_waste;        /* Need to waste some elements */

/* Mesh data */
extern int *znn;              /* Z direction node numbers */
extern float *z;              /* Z coordinates */

/* Plotting */
extern int pl_debug;          /* Plot debug flag */
extern int sel_log;           /* Select log scale */ 
extern int yflip;             /* Flip y axis */
extern char *title;           /* Plot title */
extern char *label;           /* Plot label */
extern int norm_style;        /* Normal style for vectors */
extern char **MatNames;       /* Material names */
extern int *tripts;          /* Triangle points */

/* Version */
extern char *VersionString;   /* Version string */

/* Temperature tracking */
extern float last_temp;      /* Last temperature */

#endif /* GLOBAL_H */
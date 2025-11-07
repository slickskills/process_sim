#!/usr/bin/env python3
"""
Simple plotting utility for heat2d results.
Usage:
    python3 tools/plot_heat2d.py mesh.msh heat2d_result.csv --out heat2d.png

The script reads a Gmsh v2 ASCII mesh (triangles) and a CSV with columns x,y,u
(in the same node order), then produces a tricontourf plot and saves to PNG.
"""
import sys
import argparse
import csv
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.tri as mtri
import numpy as np


def read_msh_v2(filename):
    nodes = []  # list of (id, x, y)
    triangles = []  # list of (n1_id, n2_id, n3_id)
    with open(filename, 'r') as f:
        lines = f.readlines()
    i = 0
    while i < len(lines):
        line = lines[i].strip()
        if line == '$Nodes':
            i += 1
            n = int(lines[i].strip())
            i += 1
            for _ in range(n):
                parts = lines[i].strip().split()
                nid = int(parts[0])
                x = float(parts[1]); y = float(parts[2])
                nodes.append((nid, x, y))
                i += 1
            # skip $EndNodes
        elif line == '$Elements':
            i += 1
            ne = int(lines[i].strip())
            i += 1
            for _ in range(ne):
                parts = lines[i].strip().split()
                eid = int(parts[0]); etype = int(parts[1]); numTags = int(parts[2])
                idx = 3
                tags = []
                for t in range(numTags):
                    tags.append(int(parts[idx])); idx += 1
                if etype == 2:
                    # triangle expected: next three ints
                    n1 = int(parts[idx]); n2 = int(parts[idx+1]); n3 = int(parts[idx+2])
                    triangles.append((n1, n2, n3))
                i += 1
        else:
            i += 1
    return nodes, triangles


def read_csv_xyu(filename):
    xs = []
    ys = []
    us = []
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        header = next(reader)
        for row in reader:
            if not row: continue
            x = float(row[0]); y = float(row[1]); u = float(row[2])
            xs.append(x); ys.append(y); us.append(u)
    return np.array(xs), np.array(ys), np.array(us)


def main():
    p = argparse.ArgumentParser()
    p.add_argument('mesh')
    p.add_argument('csv')
    p.add_argument('--out', default='heat2d.png')
    args = p.parse_args()

    nodes, triangles = read_msh_v2(args.mesh)
    xs_csv, ys_csv, us = read_csv_xyu(args.csv)

    # Build node id -> index map from nodes list (order in msh)
    id_to_idx = {nid: idx for idx, (nid,x,y) in enumerate(nodes)}

    # Now create triangle array of indices compatible with CSV order
    tri_idxs = []
    for (n1,n2,n3) in triangles:
        try:
            i1 = id_to_idx[n1]
            i2 = id_to_idx[n2]
            i3 = id_to_idx[n3]
        except KeyError:
            # fallback: try to find nearest points in CSV (shouldn't happen for our pipeline)
            continue
        tri_idxs.append((i1, i2, i3))

    tri = mtri.Triangulation(xs_csv, ys_csv, np.array(tri_idxs))

    fig, ax = plt.subplots(figsize=(5,5))
    tcf = ax.tricontourf(tri, us, levels=14, cmap='inferno')
    ax.triplot(tri, color='k', linewidth=0.3, alpha=0.6)
    ax.set_aspect('equal')
    plt.colorbar(tcf, ax=ax, label='u')
    ax.set_title('Heat2D result')
    plt.tight_layout()
    plt.savefig(args.out, dpi=200)
    print('Wrote', args.out)

if __name__ == '__main__':
    main()

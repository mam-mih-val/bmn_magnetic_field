#!/usr/bin/python

import sys, getopt

from enum import unique
import matplotlib.pyplot as plt
import matplotlib.tri as mtri
import numpy as np
import data_parser
import field_point
import field_plane
from matplotlib.backends.backend_pdf import PdfPages

def main(argv):
    inputfile = ''
    outputfile = ''
    startpos=''
    try:
        opts, args = getopt.getopt(argv,"hi:o:s:",["ifile=","ofile=","start-position"])
    except getopt.GetoptError:
        print('Error, use: draw.py -i <inputfile> -o <outputfile>')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
                print('draw.py -i <inputfile> -o <outputfile>')
                sys.exit()
        elif opt in ("-i", "--ifile"):
                inputfile = arg
        elif opt in ("-o", "--ofile"):
                outputfile = arg
        elif opt in ("-s", "--start-position"):
                startpos = arg
    input_files = inputfile.split(",")
    start_pos = startpos.split(",")

    plane = field_plane.plane([])
    for i in range(0, len(input_files)):
        points = data_parser.parse_data(input_files[i])
        temp_plane = field_plane.plane(points)
        temp_plane.shift_y(float(start_pos[i]))
        plane.points+=temp_plane.points

    plane = plane * 1000.0
    plane.voltage_to_field_bx(23.16, 136.5)
    plane.voltage_to_field_by(23.16, 136.5)
    plane.voltage_to_field_bz(23.16, 136.5)
    plane = plane / 1000.0
    unique_y = plane.unique_coordinates(field_point.y_coordinate())
    unique_y = sorted(unique_y)
    x_values = plane.get_values( field_point.x_coordinate() )
    y_values = plane.get_values( field_point.y_coordinate() )
    bx_values = plane.get_values( field_point.bx_field() )
    by_values = plane.get_values( field_point.by_field() )
    bz_values = plane.get_values( field_point.bz_field() )
    pp = PdfPages(outputfile)
    fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
    ax.plot_trisurf(x_values, y_values, bx_values, None, cmap=plt.cm.YlGnBu_r)
    ax.view_init(elev=45, azim=-120)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Bx')
    pp.savefig(fig)
    fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
    ax.plot_trisurf(x_values, y_values, by_values, None, cmap=plt.cm.YlGnBu_r)
    ax.view_init(elev=45, azim=-120)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('By')
    pp.savefig(fig)
    fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
    ax.plot_trisurf(x_values, y_values, bz_values, None, cmap=plt.cm.YlGnBu_r, label = "Bx")
    ax.view_init(elev=45, azim=-120)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Bz')
    pp.savefig(fig)

    for y in unique_y:
        slice = plane.select_points( field_point.y_equals(y) )
        x_values = slice.get_values( field_point.x_coordinate() )
        bx_values = slice.get_values( field_point.bx_field() )
        by_values = slice.get_values( field_point.by_field() )
        bz_values = slice.get_values( field_point.bz_field() )
        fig = plt.figure()
        plt.plot(x_values, bx_values, label = "Bx")
        plt.plot(x_values, by_values, label = "By")
        plt.plot(x_values, bz_values, label = "Bz")
        plt.xlabel('x (mm)')
        plt.ylabel('B (kGs)')
        plt.title('y='+str(y))
        ax = plt.gca()
        ax.set_xlim([0.0, 3000])
        ax.set_ylim([-6, 15])
        plt.legend()
        pp.savefig(fig)

    pp.close()

if __name__ == "__main__":
   main(sys.argv[1:])
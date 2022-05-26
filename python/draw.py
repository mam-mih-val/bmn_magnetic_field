#!/usr/bin/python

import sys, getopt

from enum import unique
import matplotlib.pyplot as plt
import matplotlib.tri as mtri
import numpy as np
import data_parser
import field_point
import field_plane
import progress_bar
from matplotlib.backends.backend_pdf import PdfPages


def main(argv):
    inputfile = ''
    outputfile = ''
    try:
        opts, args = getopt.getopt(argv, "hi:o:", ["ifile=", "ofile=", "start-position"])
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
    input_files = inputfile.split(",")

    plane = field_plane.plane([])
    shift = 0
    for i in range(0, len(input_files)):
        points = data_parser.parse_data(input_files[i])
        temp_plane = field_plane.plane(points)
        unique_y = plane.unique_coordinates(field_point.y_coordinate())
        unique_y = sorted(unique_y)
        if i > 0:
            shift = unique_y[-1]
        print("The start position of the points is y =", shift)
        temp_plane.shift_y(float(shift))
        if len(input_files) > 1:
            plane = plane.delete_points(field_point.y_equals(shift))
        plane.points += temp_plane.points

    unique_y = plane.unique_coordinates(field_point.y_coordinate())
    unique_y = sorted(unique_y)
    if len(input_files) > 1:
        plane = plane.delete_points(field_point.y_equals(unique_y[-1]))
    unique_x = plane.unique_coordinates(field_point.x_coordinate())
    unique_x = sorted(unique_x)
    unique_y = plane.unique_coordinates(field_point.y_coordinate())
    unique_y = sorted(unique_y)
    unique_z = plane.unique_coordinates(field_point.z_coordinate())
    unique_z = sorted(unique_z)

    plane = plane * 1000.0
    plane.voltage_to_field_bx(23.16, 136.5)
    plane.voltage_to_field_by(23.16, 136.5)
    plane.voltage_to_field_bz(23.16, 136.5)
    plane = plane / 1000.0
    unique_y = plane.unique_coordinates(field_point.y_coordinate())
    unique_y = sorted(unique_y)
    x_values = plane.get_values(field_point.x_coordinate())
    y_values = plane.get_values(field_point.y_coordinate())
    bx_values = plane.get_values(field_point.bx_field())
    by_values = plane.get_values(field_point.by_field())
    bz_values = plane.get_values(field_point.bz_field())
    pp = PdfPages(outputfile)

    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    plt.text(0.1, 0.85, 'Measurement summary', horizontalalignment='left', verticalalignment='center', transform=ax.transAxes)
    plt.text(0.1, 0.75, 'N points ' + str(len(plane.points)), horizontalalignment='left', verticalalignment='center', transform=ax.transAxes)
    plt.text(0.1, 0.65, 'x = ' + str(unique_x[0]) + " - " + str(unique_x[-1]), horizontalalignment='left', verticalalignment='center', transform=ax.transAxes)
    plt.text(0.1, 0.55, 'y = ' + str(unique_y[0]) + " - " + str(unique_y[-1]), horizontalalignment='left', verticalalignment='center', transform=ax.transAxes)
    plt.text(0.1, 0.45, 'z = ' + str(unique_z[0]) + " - " + str(unique_z[-1]), horizontalalignment='left', verticalalignment='center', transform=ax.transAxes)
    pp.savefig(fig)

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
    ax.plot_trisurf(x_values, y_values, bz_values, None, cmap=plt.cm.YlGnBu_r, label="Bx")
    ax.view_init(elev=45, azim=-120)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Bz')
    pp.savefig(fig)
    print("Plotting the x-projections of the plane...")
    pb = progress_bar.bar(40)
    pb.print()
    idx = 0
    for y in unique_y:
        slice = plane.select_points(field_point.y_equals(y))
        x_values = slice.get_values(field_point.x_coordinate())
        bx_values = slice.get_values(field_point.bx_field())
        by_values = slice.get_values(field_point.by_field())
        bz_values = slice.get_values(field_point.bz_field())
        fig = plt.figure()
        plt.plot(x_values, bx_values, label="Bx")
        plt.plot(x_values, by_values, label="By")
        plt.plot(x_values, bz_values, label="Bz")
        plt.xlabel('x (mm)')
        plt.ylabel('B (kGs)')
        plt.title('y=' + str(y))
        ax = plt.gca()
        ax.set_xlim([0.0, 3000])
        ax.set_ylim([-6, 15])
        plt.legend()
        pp.savefig(fig)
        plt.cla()
        plt.clf()
        plt.close(fig)
        idx += 1
        pb.update(float(idx / len(unique_y)))

    print("Plotting the y-projections of the plane...")
    pb = progress_bar.bar(40)
    pb.print()
    idx = 0
    for x in unique_x:
        slice = plane.select_points(field_point.x_equals(x))
        y_values = slice.get_values(field_point.y_coordinate())
        bx_values = slice.get_values(field_point.bx_field())
        by_values = slice.get_values(field_point.by_field())
        bz_values = slice.get_values(field_point.bz_field())
        fig = plt.figure()
        plt.plot(y_values, bx_values, label="Bx")
        plt.plot(y_values, by_values, label="By")
        plt.plot(y_values, bz_values, label="Bz")
        plt.xlabel('y (mm)')
        plt.ylabel('B (kGs)')
        plt.title('y=' + str(x))
        ax = plt.gca()
        ax.set_xlim([0.0, 6000])
        ax.set_ylim([-6, 15])
        plt.legend()
        pp.savefig(fig)
        plt.cla()
        plt.clf()
        plt.close(fig)
        pb.update(float(idx / len(unique_x)))
        idx += 1

    pp.close()
    print("All the info was saved in file "+outputfile)


if __name__ == "__main__":
    main(sys.argv[1:])

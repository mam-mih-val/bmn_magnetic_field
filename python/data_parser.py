import csv
import field_point


def parse_data(file_name):
    points_list = []
    with open(file_name) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter='\t')
        line_count = 0
        for row in csv_reader:
            if line_count == 0:
                line_count += 1
                continue
            else:
                points_list.append(field_point.point(row[1], row[2], row[3], row[5], row[6], row[7]))
    print("File:", file_name, ":", len(points_list), " points were read")
    return points_list

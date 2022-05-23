from ast import Raise
import field_point


class plane:
    points = []

    def __init__(self, points_):
        self.points = points_

    def voltage_to_field_bx(self, p0, p1):
        for i in range(0, len(self.points)):
            self.points[i].bx = float(p0 + p1 * self.points[i].bx)

    def voltage_to_field_by(self, p0, p1):
        for i in range(0, len(self.points)):
            self.points[i].by = float(p0 + p1 * self.points[i].by)

    def voltage_to_field_bz(self, p0, p1):
        for i in range(0, len(self.points)):
            self.points[i].bz = float(p0 + p1 * self.points[i].bz)

    def shift_x(self, shift):
        for i in range(0, len(self.points)):
            self.points[i].x += shift

    def shift_y(self, shift):
        for i in range(0, len(self.points)):
            self.points[i].y += shift

    def shift_z(self, shift):
        for i in range(0, len(self.points)):
            self.points[i].z += shift

    def get_values(self, value):
        values = []
        for p in self.points:
            values.append(value(p))
        return values

    def unique_coordinates(self, coordinate):
        unique_x = set()
        for p in self.points:
            unique_x.add(coordinate(p))
        return unique_x

    def select_points(self, criteria):
        selected_points = []
        for p in self.points:
            if criteria(p):
                selected_points.append(p)
        return plane(selected_points)

    def delete_points(self, criteria):
        selected_points = []
        for p in self.points:
            if not(criteria(p)):
                selected_points.append(p)
        return plane(selected_points)

    def __len__(self):
        return len(self.points)

    def __getitem__(self, i):
        return self.points[i]

    def __add__(self, other):
        result = plane(self.points)
        if len(self.points) != len(other.points):
            Raise("The planes are not the same length")
        for i in range(0, len(self.points)):
            result.points[i] = self.points[i] + other[i]
        return result

    def __sub__(self, other):
        result = plane(self.points)
        if len(self.points) != len(other.points):
            Raise("The planes are not the same length")
        for i in range(0, len(self.points)):
            result.points[i] = self.points[i] - other[i]
        return result

    def __mul__(self, other):
        result = plane(self.points)
        if type(other) == float or type(other) == int:
            for i in range(0, len(self.points)):
                result.points[i] = self.points[i] * other
        else:
            if len(self.points) != len(other.points):
                Raise("The planes are not the same length")
            for i in range(0, len(self.points)):
                result.points[i] = self.points[i] * other[i]
        return result

    def __truediv__(self, other):
        result = plane(self.points)
        if type(other) == float or type(other) == int:
            for i in range(0, len(self.points)):
                result.points[i] = self.points[i] / other
        else:
            if len(self.points) != len(other.points):
                Raise("The planes are not the same length")
            for i in range(0, len(self.points)):
                result.points[i] = self.points[i] / other[i]
        return result

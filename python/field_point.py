from ast import Raise
from re import X


class point:
	def __init__(self, xx, yy, zz, bxx, byy, bzz):
		self.x = float(xx)
		self.y = float(yy)
		self.z = float(zz)
		self.bx = float(bxx)
		self.by = float(byy)
		self.bz = float(bzz)

	def print(self):
		print( "Coordinate: {", self.x, self.y, self.z, "}\tField: {", self.bx, self.by, self.bz, "}"  )
	
	def __add__(self, other):
		if( self.x != other.x ):
			Raise( "X coordinates are not the same" )
		if( self.y != other.y ):
			Raise( "Y coordinates are not the same" )
		if( self.z != other.z ):
			Raise( "Z coordinates are not the same" )
		return point(self.x, self.y, self.z, self.bx+other.bx, self.by+other.by, self.bz+other.bz)
	
	def __sub__(self, other):
		if( self.x != other.x ):
			Raise( "X coordinates are not the same" )
		if( self.y != other.y ):
			Raise( "Y coordinates are not the same" )
		if( self.z != other.z ):
			Raise( "Z coordinates are not the same" )
		return point(self.x, self.y, self.z, self.bx-other.bx, self.by-other.by, self.bz-other.bz)

	def __mul__(self, other):
		if( type(other) == float or type(other) == int ):
			return point(self.x, self.y, self.z, self.bx*other, self.by*other, self.bz*other)
		else:
			if( self.x != other.x ):
				Raise( "X coordinates are not the same" )
			if( self.y != other.y ):
				Raise( "Y coordinates are not the same" )
			if( self.z != other.z ):
				Raise( "Z coordinates are not the same" )
			return point(self.x, self.y, self.z, self.bx*other.bx, self.by*other.by, self.bz*other.bz)
	def __truediv__(self, other):
		if( type(other) == float or type(other) == int ):
			return point(self.x, self.y, self.z, self.bx/other, self.by/other, self.bz/other)
		else:
			if( self.x != other.x ):
				Raise( "X coordinates are not the same" )
			if( self.y != other.y ):
				Raise( "Y coordinates are not the same" )
			if( self.z != other.z ):
				Raise( "Z coordinates are not the same" )
			return point(self.x, self.y, self.z, self.bx/other.bx, self.by/other.by, self.bz/other.bz)

class x_coordinate:
	def __call__(self, p):
		return p.x
class y_coordinate:
	def __call__(self, p):
		return p.y
class z_coordinate:
	def __call__(self, p):
		return p.z

class x_equals:
	def __init__(self, xx):
		self.x = xx
	def __call__(self, p):
		return p.x == self.x
class y_equals:
	def __init__(self, yy):
		self.y = yy
	def __call__(self, p):
		return p.y == self.y
class z_equals:
	def __init__(self, zz):
		self.z = zz
	def __call__(self, p):
		return p.z == self.z

class bx_field:
	def __call__(self, p):
		return p.bx
class by_field:
	def __call__(self, p):
		return p.by
class bz_field:
	def __call__(self, p):
		return p.bz
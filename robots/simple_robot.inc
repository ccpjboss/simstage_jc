define laser ranger(
	sensor(
		range [0.0 5.6]
		fov 240.0
		samples 240
	)
	color "blue"
	size [ 0.06 0.06 0.06]

	block( points 4
		point[0] [0 0]
		point[1] [0 1]
		point[2] [1 1]
		point[3] [1 0]
		z [0 1]
	)
)

define cam camera(
	# laser properties
  	resolution [ 1280 720 ]
  	range [ 0.5 8.0 ]
  	fov [ 100.0 70.0 ]
 	pantilt [ 0.0 0.0 ]

  	# model properties
	size [ 0.06 0.06 0.06]
  	color "black"
)

define simple_robot position(
	odom_error [0.03 0.03 0.00 0.05]
	localization "odom"
	size [0.4 0.3 0.2]
	gui_nose 1
	drive "diff"
	color "red"

	laser( pose [0.23 0.0 -0.15 0.0] )
	cam( pose [0.17 0.0 0.0 0.0] )
)


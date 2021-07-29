define frente ranger
(
    sensor (
        pose [0.25 0 -0.25 0]
        size [0.01 0.05 0.01 ] 
        range [0 3]
        fov 10
    )
)

define frente_direita ranger
(
    sensor (
        pose [0.175 -0.2 -0.25 -30]
        size [0.01 0.05 0.01 ] 
        range [0 3]
        fov 10
    )
)

define frente_esquerda ranger
(
    sensor (
        pose [0.175 0.2 -0.25 30]
        size [0.01 0.05 0.01 ] 
        range [0 3]
        fov 10
    )
)

define esquerda ranger(
    sensor (
        pose [0 0.25 -0.25 90]
        size [0.01 0.05 0.01 ] 
        range [0 3]
        fov 10
    )
)

define direita ranger(
    sensor (
        pose [0 -0.25 -0.25 -90]
        size [0.01 0.05 0.01 ] 
        range [0 3]
        fov 10
    )
)

define wall_following_robot position(
    block
      (
            points 6
            point[0] [0.375 0]
            point[1] [0.5 0.125]
            point[2] [0.5 0.375]
            point[3] [0.375 0.5]
            point[4] [0 0.5]
            point[5] [0 0]
            z [0 0.5]
      )

	odom_error [0.03 0.03 0.00 0.05]
	localization "odom"
	size [0.5 0.5 0.5]
	gui_nose 1
	drive "diff"
	color "red"
    
    frente()
    esquerda()
    direita()
    frente_direita()
    frente_esquerda()
)
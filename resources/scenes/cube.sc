{
	"ambient_light" : 0.3,
	"camera": {
		"position": [0, 2, -7],
		"angles": [0, 0, 0]
	},
	"figures": [
		{
			"type": "cube",
			"center": [0, 0, -1],
			"center2": [0, 0, -2],
			"radius": 2,
			"radius2": 1,
			"color": [1, 1, 1],
			"material": "diffuse",
			"texture": "resources/textures/test.png"
		},
		{
			"type": "plane",
			"center": [0, 6, 0],
			"center2": [0, -1, 0],
			"color": [0.9, 0.67, 0.44],
			"emission": 1,
			"material": "emissive"
		},
		{
			"type": "plane",
			"center": [0, 0, 2],
			"center2": [0, 0, 1],
			"color": [0.2, 0.2, 0.2],
			"material": "diffuse"
		},
		{
			"type": "plane",
			"center": [-3, 0, 0],
			"center2": [1, 0, 0],
			"color": [0.5, 0.1, 0],
			"material": "diffuse",
			"texture": "resources/textures/test.png"
		},
		{
			"type": "plane",
			"center": [3, 0, 0],
			"center2": [1, 0, 0],
			"color": [0, 0.1, 0.5],
			"material": "diffuse"
		},
		{
			"type": "plane",
			"center": [0, -1, 0],
			"center2": [0, 1, 0],
			"color": [0.9, 0.67, 0.44],
			"material": "diffuse"
		}
	]
}

{
	"camera": {
		"position": {
			"x": 1,
			"y": 2,
			"z": 3
		},
		"angles": {
			"x": 4,
			"y": 5,
			"z": 6
		},
		"distance": 1
	},
	"figures": [
		{
			"type": "sphere",
			"center": {
				"x": -300,
				"y": -180,
				"z": 1100
			},
			"emission": {
				"x": 1,
				"y": 2,
				"z": 3
			},
			"radius": 170,
			"color": {
				"x": 255,
				"y": 0,
				"z": 0
			},
			"texture": "./resources/images/icon.png",
			"shine": 0,
			"reflection": 0,
			"transparency": 0
		},
		{
			"type": "cylinder",
			"center": {
				"x": 0,
				"y": -350,
				"z": 1500
			},
			"center2": {
				"x": 0,
				"y": 350,
				"z": 1500
			},
			"emission": {
				"x": 1,
				"y": 2,
				"z": 3
			},
			"radius": 150,
			"color": {
				"x": 0,
				"y": 255,
				"z": 0
			},
			"texture": "./resources/images/icon  hello.png",
			"shine": 0,
			"reflection": 0,
			"transparency": 0
		},
		{
			"type": "cone",
			"center": {
				"x": 300,
				"y": -350,
				"z": 1100
			},
			"center2": {
				"x": 300,
				"y": 350,
				"z": 1100
			},
			"emission": {
				"x": 1,
				"y": 2,
				"z": 3
			},
			"angle": 20,
			"color": {
				"x": 0,
				"y": 0,
				"z": 255
			},
			"texture": "./resources/images/icon.png",
			"shine": 0,
			"reflection": 0,
			"transparency": 0
		},
		{
			"type": "plane",
			"center": {
				"x": 0,
				"y": -350,
				"z": 0
			},
			"normal": {
				"x": 0,
				"y": 1,
				"z": 0
			},
			"emission": {
				"x": 1,
				"y": 2,
				"z": 3
			},
			"color": {
				"x": 255,
				"y": 255,
				"z": 255
			},
			"texture": "./resources/images/icon.png",
			"shine": 0,
			"reflection": 0,
			"transparency": 0
		}
	],
	"lights": [
		{
			"type": "ambient",
			"power": 20
		},
		{
			"type": "point",
			"power": 80,
			"position": {
				"x": -350,
				"y": 350,
				"z": 700.99
			}
		}
	]
}

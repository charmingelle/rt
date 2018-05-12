{
	"camera": {
		"position": {
			"x": 0,
			"y": 0,
			"z": 0
		},
		"angles": {
			"x": 0,
			"y": 0,
			"z": 0
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
			"radius": 170,
			"color": "679327",
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
			"radius": 150,
			"color": "D8C077"
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
			"angle": 20,
			"color": "00FF00"
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
			"color": "AADA64"
		}
	],
	"lights": [
		{
			"type": "ambient",
			"power": 20
		},
		{
			"type": "point",
			"power": 50,
			"position": {
				"x": -350,
				"y": 350,
				"z": 700
			}
		},
		{
			"type": "directional",
			"power": 30,
			"position": {
				"x": -350,
				"y": 350,
				"z": 700
			},
			"direction": {
				"x": -1,
				"y": -2,
				"z": 1
			}
		}
	]
}

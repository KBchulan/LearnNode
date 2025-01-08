extends Node2D

var label: Label

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	label = Label.new()
	label.text = "Hello, World!"
	add_child(label)
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	label.set_position(Vector2(label.position.x + delta * 0.2,
		 label.position.y + delta * 0.2))
	pass

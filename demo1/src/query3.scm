type: [(primitive_type) (type_identifier)] @type
declarator: ([
  (pointer_declarator
  	declarator: (_)
  ) @decl_p
  (array_declarator
  	declarator: (_)
  	size: (_) @size
  ) @decl_a
  (field_identifier) @decl_f
])

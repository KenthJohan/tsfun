(type_definition
  (struct_specifier
    (field_declaration_list
      (field_declaration
        (type_qualifier)? 
        [(primitive_type) @type (type_identifier) @type]
        (field_identifier) @field
      )+
    )
  )
  (type_identifier) @name
)
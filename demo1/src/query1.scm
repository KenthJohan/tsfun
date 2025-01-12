(type_definition
  (struct_specifier
    (field_declaration_list
      (field_declaration
        (type_qualifier)? 
        [(primitive_type) (type_identifier)] @field_type
        (field_identifier) @field_identifier
      )+ @field
    )
  )
  (type_identifier) @struct_name
)
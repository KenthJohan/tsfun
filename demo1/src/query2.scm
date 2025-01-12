(type_definition
  (struct_specifier
    (field_declaration_list
      (comment)? @field_comment
      (field_declaration)+ @field
    )
  )
  (type_identifier) @name
)
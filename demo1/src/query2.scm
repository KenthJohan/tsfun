(type_definition
  (struct_specifier
    (field_declaration_list
      (comment)? @field_comment
      (field_declaration
        (type_qualifier)? 
        type: (_) @type
        declarator: (_) @decl
      )+ @field
    )
  )
  (type_identifier) @name
)
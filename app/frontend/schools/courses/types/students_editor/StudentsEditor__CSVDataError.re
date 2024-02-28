type nonrec errorType =
  | Name
  | Email
  | Title
  | TeamName
  | Tags
  | Affiliation;
type nonrec t = {
  rowNumber: int,
  errors: array(errorType),
};
let rowNumber = t => t.rowNumber;
let errors = t => t.errors;
let nameError = data =>
  switch (StudentsEditor__StudentCSVData.name(data)) {
  | None => [||]
  | Some(name) =>
    if (StringUtils.isPresent(name)) {
      [||];
    } else {
      [|Name|];
    }
  };
let emailError = data =>
  switch (StudentsEditor__StudentCSVData.email(data)) {
  | None => [|Email|]
  | Some(email) =>
    if (EmailUtils.isInvalid(false, email)) {
      [|Email|];
    } else {
      [||];
    }
  };
let titleError = data =>
  switch (StudentsEditor__StudentCSVData.title(data)) {
  | None => [||]
  | Some(title) =>
    if (String.length(title) <= 250) {
      [||];
    } else {
      [|Title|];
    }
  };
let affiliationError = data =>
  switch (StudentsEditor__StudentCSVData.affiliation(data)) {
  | None => [||]
  | Some(affiliation) =>
    if (String.length(affiliation) <= 250) {
      [||];
    } else {
      [|Affiliation|];
    }
  };
let teamNameError = data =>
  switch (StudentsEditor__StudentCSVData.teamName(data)) {
  | None => [||]
  | Some(teamName) =>
    if (String.length(teamName) <= 50) {
      [||];
    } else {
      [|TeamName|];
    }
  };
let tagsError = data =>
  switch (StudentsEditor__StudentCSVData.tags(data)) {
  | None => [||]
  | Some(tagsList) =>
    let tags = Js.String.split(~sep=",", tagsList);
    let validTags =
      tags |> Js.Array.filter(~f=tag => String.length(tag) <= 50);

    if (tags->Array.length <= 5 && validTags == tags) {
      [||];
    } else {
      [|Tags|];
    };
  };
let parseError = studentCSVData =>
  studentCSVData
  |> Js.Array.mapi(~f=(data, index) => {
       let errors =
         ArrayUtils.flattenV2([|
           nameError(data),
           emailError(data),
           titleError(data),
           affiliationError(data),
           teamNameError(data),
           tagsError(data),
         |]);

       if (errors |> ArrayUtils.isEmpty) {
         [||];
       } else {
         [|{rowNumber: index + 2, errors}|];
       };
     })
  |> ArrayUtils.flattenV2;
let hasNameError = t =>
  t.errors
  |> Js.Array.filter(~f=x =>
       switch (x) {
       | Name => true
       | _ => false
       }
     )
  |> ArrayUtils.isNotEmpty;
let hasTitleError = t =>
  t.errors
  |> Js.Array.filter(~f=x =>
       switch (x) {
       | Title => true
       | _ => false
       }
     )
  |> ArrayUtils.isNotEmpty;
let hasEmailError = t =>
  t.errors
  |> Js.Array.filter(~f=x =>
       switch (x) {
       | Email => true
       | _ => false
       }
     )
  |> ArrayUtils.isNotEmpty;
let hasAffiliationError = t =>
  t.errors
  |> Js.Array.filter(~f=x =>
       switch (x) {
       | Affiliation => true
       | _ => false
       }
     )
  |> ArrayUtils.isNotEmpty;
let hasTeamNameError = t =>
  t.errors
  |> Js.Array.filter(~f=x =>
       switch (x) {
       | TeamName => true
       | _ => false
       }
     )
  |> ArrayUtils.isNotEmpty;
let hasTagsError = t =>
  t.errors
  |> Js.Array.filter(~f=x =>
       switch (x) {
       | Tags => true
       | _ => false
       }
     )
  |> ArrayUtils.isNotEmpty;

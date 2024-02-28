type nonrec choices = array(string);
let t = I18n.t(~scope="components.TargetChecklistItem");
type nonrec kind =
  | Files
  | Link
  | ShortText
  | LongText
  | MultiChoice(choices)
  | AudioRecord;
type nonrec t = {
  title: string,
  kind,
  optional: bool,
};
let title = t => t.title;
let kind = t => t.kind;
let optional = t => t.optional;
let actionStringForKind = kind =>
  switch (kind) {
  | Files => t("action_string_upload_files")
  | Link => t("action_string_attach_link")
  | ShortText => t("action_string_write_short_text")
  | LongText => t("action_string_write_long_text")
  | MultiChoice(_choices) => t("action_string_choose_from_list")
  | AudioRecord => t("action_string_record_audio")
  };
let kindAsString = kind =>
  switch (kind) {
  | Files => "files"
  | Link => "link"
  | ShortText => "shortText"
  | AudioRecord => "audio"
  | LongText => "longText"
  | MultiChoice(_choices) => "multiChoice"
  };
let make = (~title, ~kind, ~optional) => {title, kind, optional};
let updateTitle = (title, t) => {...t, title};
let updateKind = (kind, t) => {...t, kind};
let updateOptional = (optional, t) => {...t, optional};
let removeItem = (index, array) =>
  array |> Js.Array.filteri(~f=(_item, i) => i != index);
let moveUp = (index, array) => array |> ArrayUtils.swapUp(index);
let moveDown = (index, array) => array |> ArrayUtils.swapDown(index);
let copy = (index, array) =>
  array
  |> Js.Array.mapi(~f=(item, i) =>
       if (i == index) {
         [|item, item|];
       } else {
         [|item|];
       }
     )
  |> ArrayUtils.flattenV2;
let removeMultichoiceOption = (choiceIndex, t) =>
  switch (t.kind) {
  | MultiChoice(choices) =>
    let updatedChoices =
      choices
      |> Array.mapi((i, choice) =>
           if (i == choiceIndex) {
             [||];
           } else {
             [|choice|];
           }
         )
      |> ArrayUtils.flattenV2;
    updateKind(MultiChoice(updatedChoices), t);
  | Files
  | Link
  | AudioRecord
  | ShortText
  | LongText => t
  };
let addMultichoiceOption = t =>
  switch (t.kind) {
  | MultiChoice(choices) =>
    let updatedChoices = Js.Array.concat(~other=[|""|], choices);
    updateKind(MultiChoice(updatedChoices), t);
  | Files
  | Link
  | ShortText
  | AudioRecord
  | LongText => t
  };
let updateMultichoiceOption = (choiceIndex, newOption, t) =>
  switch (t.kind) {
  | MultiChoice(choices) =>
    let updatedChoices =
      choices
      |> Js.Array.mapi(~f=(choice, i) =>
           if (i == choiceIndex) {
             newOption;
           } else {
             choice;
           }
         );
    updateKind(MultiChoice(updatedChoices), t);
  | Files
  | Link
  | ShortText
  | AudioRecord
  | LongText => t
  };
let longText = {title: "", kind: LongText, optional: false};
let isFilesKind = t =>
  switch (t.kind) {
  | Files => true
  | MultiChoice(_choices) => false
  | Link
  | ShortText
  | AudioRecord
  | LongText => false
  };
let isValidChecklistItem = t => {
  let titleValid = Js.String.trim(t.title) |> Js.String.length >= 1;
  switch (t.kind) {
  | MultiChoice(choices) =>
    choices
    |> Js.Array.filter(~f=choice => String.trim(choice) == "")
    |> ArrayUtils.isEmpty
    && titleValid
  | Files
  | Link
  | ShortText
  | AudioRecord
  | LongText => titleValid
  };
};
let decodeMetadata = (kind, json) =>
  switch (kind) {
  | `MultiChoice => json |> Json.Decode.(field("choices", array(string)))
  };
let decode = json =>
  Json.Decode.{
    kind:
      switch (json |> field("kind", string)) {
      | "files" => Files
      | "link" => Link
      | "shortText" => ShortText
      | "audio" => AudioRecord
      | "longText" => LongText
      | "multiChoice" =>
        MultiChoice(json |> field("metadata", decodeMetadata(`MultiChoice)))
      | otherKind =>
        Rollbar.error(
          "Unkown kind: "
          ++ otherKind
          ++ "received in CurriculumEditor__TargetChecklistItem",
        );
        LongText;
      },
    optional: json |> field("optional", bool),
    title: json |> field("title", string),
  };
let encodeMetadata = kind =>
  switch (kind) {
  | MultiChoice(choices) =>
    Json.Encode.(object_([("choices", stringArray(choices))]))
  | Files
  | Link
  | ShortText
  | AudioRecord
  | LongText => Json.Encode.(object_([]))
  };
let encode = t =>
  Json.Encode.(
    object_([
      ("kind", t.kind |> kindAsString |> string),
      ("title", t.title |> string),
      ("optional", t.optional |> bool),
      ("metadata", t.kind |> encodeMetadata),
    ])
  );
let encodeChecklist = checklist => checklist |> Json.Encode.(array(encode));

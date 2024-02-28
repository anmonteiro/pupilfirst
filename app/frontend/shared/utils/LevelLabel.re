let ts = I18n.t(~scope="shared.level_label");
let format = (~short=false, ~name=?, number) =>
  switch (name) {
  | Some(name) =>
    if (short) {
      ts(
        ~variables=[|("number", number), ("name", name)|],
        "short_with_name",
      );
    } else {
      ts(
        ~variables=[|("number", number), ("name", name)|],
        "long_with_name",
      );
    }
  | None =>
    if (short) {
      ts(~variables=[|("number", number)|], "short_without_name");
    } else {
      ts(~variables=[|("number", number)|], "long_without_name");
    }
  };
let searchString = (number, name) =>
  ts("long_with_name", ~variables=[|("number", number), ("name", name)|]);

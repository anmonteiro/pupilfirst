type nonrec t = {
  id: string,
  name: string,
  active: bool,
};
let id = t => t.id;
let name = t => t.name;
let active = t => t.active;
let decode = json =>
  Json.Decode.{
    id: json |> field("id", string),
    name: json |> field("name", string),
    active: json |> field("active", bool),
  };

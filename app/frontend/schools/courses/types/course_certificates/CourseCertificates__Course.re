type nonrec id = string;
type nonrec t = {
  id,
  name: string,
};
let id = t => t.id;
let decode = json =>
  Json.Decode.{
    id: json |> field("id", string),
    name: json |> field("name", string),
  };

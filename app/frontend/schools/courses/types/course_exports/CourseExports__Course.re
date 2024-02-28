type nonrec id = string;
type nonrec t = {id};
let id = t => t.id;
let decode = json => Json.Decode.{id: json |> field("id", string)};

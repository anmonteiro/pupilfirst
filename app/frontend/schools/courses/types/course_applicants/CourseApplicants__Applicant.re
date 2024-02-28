type nonrec t = {
  id: string,
  name: string,
  email: string,
  tags: array(string),
};
let name = t => t.name;
let tags = t => t.tags;
let id = t => t.id;
let email = t => t.email;
let make = (~id, ~name, ~tags, ~email) => {id, name, tags, email};
let makeFromJS = applicantDetails =>
  make(
    ~id=applicantDetails##id,
    ~name=applicantDetails##name,
    ~tags=applicantDetails##tags,
    ~email=applicantDetails##email,
  );
let decode = json =>
  Json.Decode.{
    id: field("id", string, json),
    name: field("name", string, json),
    email: field("email", string, json),
    tags: field("tags", array(string), json),
  };

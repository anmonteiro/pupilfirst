type nonrec t = {
  id: string,
  name: string,
  email: string,
  avatarUrl: option(string),
  title: string,
  affiliation: option(string),
  taggings: array(string),
};
let id = t => t.id;
let name = t => t.name;
let email = t => t.email;
let avatarUrl = t => t.avatarUrl;
let title = t => t.title;
let affiliation = t => t.affiliation;
let taggings = t => t.taggings;
let make = (~id, ~name, ~avatarUrl, ~title, ~email, ~affiliation, ~taggings) => {
  id,
  name,
  avatarUrl,
  title,
  email,
  affiliation,
  taggings,
};
let makeFromJs = jsObject =>
  make(
    ~id=jsObject##id,
    ~name=jsObject##name,
    ~avatarUrl=jsObject##avatarUrl,
    ~title=jsObject##title,
    ~email=jsObject##email,
    ~affiliation=jsObject##affiliation,
    ~taggings=jsObject##taggings,
  );
module Fragment = [%graphql
  {js|
  fragment AdminUserFragment on User {
    id
    name
    title
    affiliation
    email
    avatarUrl
    taggings
    lastSeenAt
  }
|js}
];
let makeFromFragment = (user: Fragment.t) => {
  id: user.id,
  name: user.name,
  title: user.title,
  affiliation: user.affiliation,
  email: user.email,
  avatarUrl: user.avatarUrl,
  taggings: user.taggings,
};

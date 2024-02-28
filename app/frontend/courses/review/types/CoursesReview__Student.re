type nonrec t = {
  id: string,
  name: string,
};
let id = t => t.id;
let name = t => t.name;
let makeFromJs = jsObject => {id: jsObject##id, name: jsObject##name};

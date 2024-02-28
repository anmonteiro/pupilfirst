type nonrec t = {
  id: string,
  title: string,
};
let id = t => t.id;
let title = t => t.title;
let make = (~id, ~title) => {id, title};
let makeFromJs = target => make(~id=target##id, ~title=target##title);

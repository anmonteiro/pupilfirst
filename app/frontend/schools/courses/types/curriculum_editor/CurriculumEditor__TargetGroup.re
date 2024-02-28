type nonrec t = {
  id: string,
  name: string,
  description: option(string),
  milestone: bool,
  levelId: string,
  sortIndex: int,
  archived: bool,
};
let id = t => t.id;
let name = t => t.name;
let description = t => t.description;
let milestone = t => t.milestone;
let levelId = t => t.levelId;
let archived = t => t.archived;
let decode = json =>
  Json.Decode.{
    id: json |> field("id", string),
    name: json |> field("name", string),
    description:
      json |> field("description", nullable(string)) |> Js.Null.toOption,
    levelId: json |> field("levelId", string),
    milestone: json |> field("milestone", bool),
    sortIndex: json |> field("sortIndex", int),
    archived: json |> field("archived", bool),
  };
let create = (id, name, description, milestone, levelId, sortIndex, archived) => {
  id,
  name,
  description,
  milestone,
  levelId,
  sortIndex,
  archived,
};
let updateArray = (targetGroups, targetGroup) =>
  targetGroups
  |> Js.Array.filter(~f=tg => tg.id != targetGroup.id)
  |> Js.Array.concat(~other=[|targetGroup|]);
let sort = targetGroups =>
  targetGroups |> ArrayUtils.copyAndSort((x, y) => x.sortIndex - y.sortIndex);
let unarchive = t => {...t, archived: false};
let unsafeFind = (targetGroups, componentName, id) =>
  targetGroups
  |> ArrayUtils.unsafeFind(
       l => l.id == id,
       "Unable to find target group with id: "
       ++ id
       ++ "in CurriculumEditor__"
       ++ componentName,
     );
let updateSortIndex = sortedTargetGroups =>
  sortedTargetGroups
  |> Js.Array.mapi(~f=(t, sortIndex) =>
       create(
         t.id,
         t.name,
         t.description,
         t.milestone,
         t.levelId,
         sortIndex,
         t.archived,
       )
     );

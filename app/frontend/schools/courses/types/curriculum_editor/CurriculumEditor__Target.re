exception InvalidVisibilityValue(string);
type nonrec visibility =
  | Draft
  | Live
  | Archived;
type nonrec t = {
  id: string,
  targetGroupId: string,
  title: string,
  sortIndex: int,
  visibility,
};
let id = t => t.id;
let title = t => t.title;
let targetGroupId = t => t.targetGroupId;
let sortIndex = t => t.sortIndex;
let visibility = t => t.visibility;
let decodeVisbility = visibilityString =>
  switch (visibilityString) {
  | "draft" => Draft
  | "live" => Live
  | "archived" => Archived
  | _ => raise(InvalidVisibilityValue("Unknown Value"))
  };
let decode = json =>
  Json.Decode.{
    id: json |> field("id", string),
    targetGroupId: json |> field("targetGroupId", string),
    title: json |> field("title", string),
    sortIndex: json |> field("sortIndex", int),
    visibility: decodeVisbility(json |> field("visibility", string)),
  };
let updateArray = (targets, target) =>
  targets
  |> Js.Array.filter(~f=t => t.id != target.id)
  |> Js.Array.concat(~other=[|target|]);
let create = (~id, ~targetGroupId, ~title, ~sortIndex, ~visibility) => {
  id,
  targetGroupId,
  title,
  sortIndex,
  visibility,
};
let sort = targets =>
  targets |> ArrayUtils.copyAndSort((x, y) => x.sortIndex - y.sortIndex);
let archive = t => {...t, visibility: Archived};
let archived = t =>
  switch (t.visibility) {
  | Archived => true
  | Live => false
  | Draft => false
  };
let removeTarget = (target, targets) =>
  targets |> Js.Array.filter(~f=t => t.id != target.id);
let targetIdsInTargetGroup = (targetGroupId, targets) =>
  targets
  |> Js.Array.filter(~f=t => t.targetGroupId == targetGroupId)
  |> Js.Array.map(~f=t => t.id);
let updateSortIndex = sortedTargets =>
  sortedTargets
  |> Js.Array.mapi(~f=(t, sortIndex) =>
       create(
         ~id=t.id,
         ~targetGroupId=t.targetGroupId,
         ~title=t.title,
         ~sortIndex,
         ~visibility=t.visibility,
       )
     );
let template = (id, targetGroupId, title) =>
  create(~id, ~targetGroupId, ~title, ~sortIndex=999, ~visibility=Draft);

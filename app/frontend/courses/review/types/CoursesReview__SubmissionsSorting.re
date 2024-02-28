type nonrec sortDirection = [ | `Ascending | `Descending];
type nonrec sortCriterion = [ | `EvaluatedAt | `SubmittedAt];
type nonrec t = {
  sortDirection,
  sortCriterion,
};
let make = (~sortDirection, ~sortCriterion) => {
  sortDirection,
  sortCriterion,
};
let default = () =>
  make(~sortCriterion=`SubmittedAt, ~sortDirection=`Ascending);
let sortDirection = t => t.sortDirection;
let sortCriterion = t => t.sortCriterion;
let updateDirection = (sortDirection, t) => {...t, sortDirection};
let updateCriterion = (sortCriterion, t) => {...t, sortCriterion};

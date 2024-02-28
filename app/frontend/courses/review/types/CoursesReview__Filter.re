type nonrec selectedTab = [ | `Reviewed | `Pending];
type nonrec sortDirection = [ | `Ascending | `Descending];
type nonrec sortCriterion = [ | `EvaluatedAt | `SubmittedAt];
type nonrec t = {
  nameOrEmail: option(string),
  levelId: option(string),
  personalCoachId: option(string),
  assignedCoachId: option(string),
  reviewingCoachId: option(string),
  targetId: option(string),
  sortCriterion,
  sortDirection: option(sortDirection),
  tab: option(selectedTab),
  includeInactive: bool,
};
let nameOrEmail = t => t.nameOrEmail;
let levelId = t => t.levelId;
let targetId = t => t.targetId;
let sortCriterion = t => t.sortCriterion;
let sortDirection = t => t.sortDirection;
let tab = t => t.tab;
let includeInactive = t => t.includeInactive;
let assignedCoachId = t => t.assignedCoachId;
let personalCoachId = t => t.personalCoachId;
let reviewingCoachId = t => t.reviewingCoachId;
let defaultDirection = t =>
  switch (t.sortDirection) {
  | Some(direction) => direction
  | None =>
    switch (t.tab) {
    | Some(tab) when tab == `Pending => `Ascending
    | _ => `Descending
    }
  };
let makeFromQueryParams = search => {
  let params = Webapi.Url.URLSearchParams.make(search);
  Webapi.Url.URLSearchParams.{
    nameOrEmail: get("search", params),
    levelId: get("levelId", params),
    personalCoachId: get("personalCoachId", params),
    assignedCoachId: get("assignedCoachId", params),
    reviewingCoachId: get("reviewingCoachId", params),
    targetId: get("targetId", params),
    tab:
      switch (get("tab", params)) {
      | Some(t) when t == "Pending" => Some(`Pending)
      | Some(t) when t == "Reviewed" => Some(`Reviewed)
      | _ => None
      },
    sortCriterion:
      switch (get("sortCriterion", params)) {
      | Some(criterion) when criterion == "EvaluatedAt" => `EvaluatedAt
      | Some(criterion) when criterion == "SubmittedAt" => `SubmittedAt
      | _ => `SubmittedAt
      },
    sortDirection:
      switch (get("sortDirection", params)) {
      | Some(direction) when direction == "Descending" => Some(`Descending)
      | Some(direction) when direction == "Ascending" => Some(`Ascending)
      | _ => None
      },
    includeInactive:
      switch (get("includeInactive", params)) {
      | Some(t) when t == "true" => true
      | _ => false
      },
  };
};
let toQueryString = filter => {
  let sortCriterion =
    switch (filter.sortCriterion) {
    | `EvaluatedAt => "EvaluatedAt"
    | `SubmittedAt => "SubmittedAt"
    };
  let filterDict = Js.Dict.fromArray([|("sortCriterion", sortCriterion)|]);
  switch (filter.sortDirection) {
  | Some(direction) =>
    Js.Dict.set(
      filterDict,
      "sortDirection",
      switch (direction) {
      | `Descending => "Descending"
      | `Ascending => "Ascending"
      },
    )
  | _ => ()
  };
  Belt.Option.forEach(filter.nameOrEmail, search =>
    Js.Dict.set(filterDict, "search", search)
  );
  Belt.Option.forEach(filter.targetId, targetId =>
    Js.Dict.set(filterDict, "targetId", targetId)
  );
  Belt.Option.forEach(filter.levelId, levelId =>
    Js.Dict.set(filterDict, "levelId", levelId)
  );
  Belt.Option.forEach(filter.personalCoachId, coachId =>
    Js.Dict.set(filterDict, "personalCoachId", coachId)
  );
  if (filter.tab != Some(`Reviewed)) {
    Belt.Option.forEach(filter.assignedCoachId, assignedCoachId =>
      Js.Dict.set(filterDict, "assignedCoachId", assignedCoachId)
    );
  };
  if (filter.tab != Some(`Pending)) {
    Belt.Option.forEach(filter.reviewingCoachId, reviewingCoachId =>
      Js.Dict.set(filterDict, "reviewingCoachId", reviewingCoachId)
    );
  };
  switch (filter.tab) {
  | Some(tab) =>
    switch (tab) {
    | `Pending => Js.Dict.set(filterDict, "tab", "Pending")
    | `Reviewed => Js.Dict.set(filterDict, "tab", "Reviewed")
    }
  | None => ()
  };

  if (filter.includeInactive) {
    Js.Dict.set(filterDict, "includeInactive", "true");
  } else {
    ();
  };
  Webapi.Url.(
    URLSearchParams.makeWithDict(filterDict)->URLSearchParams.toString
  );
};

open TeamsEditor__Types;
let t = I18n.t(~scope="components.TeamsActions__Root");
let str = React.string;
let pageLinks = studentId => [|
  School__PageHeader.makeLink(
    ~href=({js|/school/teams/|js} ++ studentId) ++ {js|/details|js},
    ~title=t("pages.links"),
    ~icon="fas fa-edit",
    ~selected=false,
    (),
  ),
  School__PageHeader.makeLink(
    ~href=({js|/school/teams/|js} ++ studentId) ++ {js|/actions|js},
    ~title=t("pages.actions"),
    ~icon="fas fa-cog",
    ~selected=true,
    (),
  ),
|];
type nonrec baseData =
  | Unloaded
  | Loading
  | Loaded(Team.t)
  | Errored;
type nonrec state = {
  baseData,
  saving: bool,
};
module TeamFragment = Team.Fragment;
module TeamDetailsDataQuery = [%graphql
  {js|
  query TeamDetailsDataQuery($id: ID!) {
    team(id: $id) {
      ...TeamFragment
    }
  }
|js}
];
let loadData = (id, setState, setCourseId) => {
  setState(state => {...state, baseData: Loading});
  TeamDetailsDataQuery.fetch(~notifyOnNotFound=false, {id: id})
  |> Js.Promise.then_((response: TeamDetailsDataQuery.t) => {
       setState(state =>
         {...state, baseData: Loaded(response.team->Team.makeFromFragment)}
       );
       setCourseId(response.team.cohort.courseId);
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(_error => {
       setState(state => {...state, baseData: Errored});
       Js.Promise.resolve();
     })
  |> ignore;
};
module DestroyTeamQuery = [%graphql
  {js|
    mutation DestroyTeamQuery($teamId: ID!) {
      destroyTeam(teamId: $teamId) {
        success
      }
    }
  |js}
];
let destroyTeam = (setState, courseId, teamId) => {
  setState(state => {...state, saving: true});
  DestroyTeamQuery.fetch(DestroyTeamQuery.makeVariables(~teamId, ()))
  |> Js.Promise.then_((result: DestroyTeamQuery.t) => {
       if (result.destroyTeam.success) {
         ReasonReactRouter.push(
           ({js|/school/courses/|js} ++ courseId) ++ {js|/teams|js},
         );
       } else {
         setState(state => {...state, saving: false});
       };
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(error => {
       Js.log(error);
       setState(state => {...state, saving: false});
       Js.Promise.resolve();
     })
  |> ignore;
};
[@react.component]
let make = (~studentId) => {
  let (state, setState) =
    React.useState(() => {baseData: Unloaded, saving: false});
  let courseContext = React.useContext(SchoolRouter__CourseContext.context);
  React.useEffect1(
    () => {
      loadData(studentId, setState, courseContext.setCourseId);
      None;
    },
    [|studentId|],
  );

  switch (state.baseData) {
  | Unloaded
  | Loading => SkeletonLoading.coursePage()
  | Loaded(team) =>
    let courseId = Team.cohort(team)->Cohort.courseId;
    <div>
      <School__PageHeader
        exitUrl={({js|/school/courses/|js} ++ courseId) ++ {js|/teams|js}}
        title={(t("edit") ++ {js| |js}) ++ Team.name(team)}
        description={t("team_actions")}
        links={pageLinks(studentId)}
      />
      <div className="max-w-4xl 2xl:max-w-5xl mx-auto px-4">
        <h2 className="text-lg font-semibold mt-8">
          {((t("delete") ++ {js| |js}) ++ Team.name(team))->str}
        </h2>
        <p className="text-sm text-gray-500">
          {t("delete_team_info")->str}
        </p>
        <button
          onClick={_ => destroyTeam(setState, courseId, Team.id(team))}
          className="btn btn-danger mt-4">
          {t("delete_team")->str}
        </button>
      </div>
    </div>;
  | Errored => <ErrorState />
  };
};

open TeamsEditor__Types;
let t = I18n.t(~scope="components.TeamsDetails__Root");
let str = React.string;
let pageLinks = studentId => [|
  School__PageHeader.makeLink(
    ~href=({js|/school/teams/|js} ++ studentId) ++ {js|/details|js},
    ~title=t("pages.details"),
    ~icon="fas fa-edit",
    ~selected=true,
    (),
  ),
  School__PageHeader.makeLink(
    ~href=({js|/school/teams/|js} ++ studentId) ++ {js|/actions|js},
    ~title=t("pages.actions"),
    ~icon="fas fa-cog",
    ~selected=false,
    (),
  ),
|];
type nonrec state =
  | Unloaded
  | Loading
  | Loaded(Team.t)
  | Errored;
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
  setState(_ => Loading);
  TeamDetailsDataQuery.fetch(~notifyOnNotFound=false, {id: id})
  |> Js.Promise.then_((response: TeamDetailsDataQuery.t) => {
       setState(_ => Loaded(response.team->Team.makeFromFragment));
       setCourseId(response.team.cohort.courseId);
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(_error => {
       setState(_ => Errored);
       Js.Promise.resolve();
     })
  |> ignore;
};
[@react.component]
let make = (~studentId) => {
  let (state, setState) = React.useState(() => Unloaded);
  let courseContext = React.useContext(SchoolRouter__CourseContext.context);
  React.useEffect1(
    () => {
      loadData(studentId, setState, courseContext.setCourseId);
      None;
    },
    [|studentId|],
  );

  switch (state) {
  | Unloaded
  | Loading => SkeletonLoading.coursePage()
  | Loaded(team) =>
    let courseId = Team.cohort(team)->Cohort.courseId;
    <div>
      <School__PageHeader
        exitUrl={({js|/school/courses/|js} ++ courseId) ++ {js|/teams|js}}
        title={(t("edit") ++ {js| |js}) ++ Team.name(team)}
        description={t("page_description")}
        links={pageLinks(studentId)}
      />
      <AdminCoursesShared__TeamEditor courseId team />
    </div>;
  | Errored => <ErrorState />
  };
};

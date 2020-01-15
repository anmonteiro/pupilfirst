[@bs.config {jsx: 3}];

let str = React.string;

open CurriculumEditor__Types;

type page =
  | Content
  | Details
  | Versions;

let tab = (page, selectedPage, pathPrefix) => {
  let defaultClasses = "curriculum-editor__drawer-tab-item cursor-pointer";

  let (number, title, pathSuffix) =
    switch (page) {
    | Content => (1, "Content", "content")
    | Details => (2, "Details", "details")
    | Versions => (3, "Versions", "versions")
    };

  let path = pathPrefix ++ pathSuffix;
  let selected = page == selectedPage;

  let classes =
    selected
      ? defaultClasses ++ " curriculum-editor__drawer-tab-item--selected"
      : defaultClasses;

  <a
    href=path
    onClick={e => {
      e |> ReactEvent.Mouse.preventDefault;
      ReasonReactRouter.push(path);
    }}
    className=classes>
    <span className="curriculum-editor__tab-step-number">
      {number |> string_of_int |> str}
    </span>
    <span className="ml-2"> {title |> str} </span>
  </a>;
};

[@react.component]
let make = (~targets, ~course) => {
  let url = ReasonReactRouter.useUrl();

  switch (url.path) {
  | ["school", "courses", _courseId, "targets", targetId, pageName] =>
    let target =
      targets
      |> ListUtils.unsafeFind(
           t => t |> Target.id == targetId,
           "Could not find target for editor drawer with the ID " ++ targetId,
         );

    let pathPrefix =
      "/school/courses/"
      ++ (course |> Course.id)
      ++ "/targets/"
      ++ targetId
      ++ "/";

    let (innerComponent, selectedPage) =
      switch (pageName) {
      | "content" => (<CurriculumEditor__ContentEditor target />, Content)
      | "details" => (
          <CurriculumEditor__TargetDetailsEditor targetId />,
          Details,
        )
      | "versions" => (
          <div> {"Target version selector goes here" |> str} </div>,
          Versions,
        )
      | otherPage =>
        Rollbar.warning(
          "Unexpected page requested for target editor drawer: " ++ otherPage,
        );
        (
          <div> {"Unexpected error. Please reload the page." |> str} </div>,
          Content,
        );
      };

    <SchoolAdmin__EditorDrawer
      size=SchoolAdmin__EditorDrawer.Large
      closeDrawerCB={() =>
        ReasonReactRouter.push(
          "/school/courses/" ++ (course |> Course.id) ++ "/curriculum",
        )
      }>
      <div>
        <div className="bg-gray-200 pt-6">
          <div
            className="flex flex-wrap w-full max-w-3xl mx-auto text-sm px-3 -mb-px">
            {tab(Content, selectedPage, pathPrefix)}
            {tab(Details, selectedPage, pathPrefix)}
            {tab(Versions, selectedPage, pathPrefix)}
          </div>
        </div>
        <div className="bg-white">
          <div className="mx-auto border-t border-gray-400">
            innerComponent
          </div>
        </div>
      </div>
    </SchoolAdmin__EditorDrawer>;
  | _otherRoutes => React.null
  };
};

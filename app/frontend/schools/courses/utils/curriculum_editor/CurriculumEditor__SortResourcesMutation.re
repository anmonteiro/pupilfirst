type resourceType =
  | Target
  | TargetGroup;
module SortResourceMutation = [%graphql
  {js|
   mutation SortCurriculumResourcesMutation($resourceIds: [ID!]!, $resourceType: String!) {
    sortCurriculumResources(resourceIds: $resourceIds, resourceType: $resourceType){
      success
    }
  }
   |js}
];
let resourceTypeToString = resourceType =>
  switch (resourceType) {
  | Target => "Target"
  | TargetGroup => "TargetGroup"
  };
let sort = (resourceType, resourceIds) =>
  SortResourceMutation.make(
    ~notify=false,
    {resourceIds, resourceType: resourceTypeToString(resourceType)},
  )
  |> Js.Promise.then_(_response => Js.Promise.resolve())
  |> ignore;

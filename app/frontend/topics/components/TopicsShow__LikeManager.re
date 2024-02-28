open TopicsShow__Types;
let str = React.string;
let t = I18n.t(~scope="components.TopicsShow__LikeManager");
module CreatePostLikeQuery = [%graphql
  {js|
  mutation CreatePostLikeMutation($postId: ID!) {
    createPostLike(postId: $postId)  {
      success
    }
  }
|js}
];
module DeletePostLikeQuery = [%graphql
  {js|
  mutation DeletePostLikeMutation($postId: ID!) {
    deletePostLike(postId: $postId) {
      success
    }
  }
  |js}
];
let iconClasses = (liked, saving) => {
  let classes = "text-lg";
  classes
  ++ (
    if (saving) {
      " fas fa-thumbs-up cursor-pointer text-primary-200";
    } else if (liked) {
      " fas fa-thumbs-up cursor-pointer text-primary-400";
    } else {
      " far fa-thumbs-up cursor-pointer";
    }
  );
};
let handlePostLike =
    (
      saving,
      liked,
      setSaving,
      postId,
      removeLikeCB,
      handleCreateResponse,
      addLikeCB,
      event,
    ) => {
  event |> React.Event.Mouse.preventDefault;

  if (saving) {
    ();
  } else {
    setSaving(_ => true);
    if (liked) {
      DeletePostLikeQuery.fetch({postId: postId})
      |> Js.Promise.then_((response: DeletePostLikeQuery.t) => {
           if (response.deletePostLike.success) {
             removeLikeCB();
             setSaving(_ => false);
           } else {
             setSaving(_ => false);
           };
           Js.Promise.resolve();
         })
      |> Js.Promise.catch(_ => {
           setSaving(_ => false);
           Js.Promise.resolve();
         })
      |> ignore;
    } else {
      CreatePostLikeQuery.fetch({postId: postId})
      |> Js.Promise.then_((response: CreatePostLikeQuery.t) => {
           if (response.createPostLike.success) {
             handleCreateResponse(setSaving, addLikeCB);
           } else {
             setSaving(_ => false);
           };
           Js.Promise.resolve();
         })
      |> Js.Promise.catch(_ => {
           setSaving(_ => false);
           Js.Promise.resolve();
         })
      |> ignore;
    };
  };
};
let handleCreateResponse = (setSaving, addLikeCB) => {
  setSaving(_ => false);
  addLikeCB();
};
[@react.component]
let make = (~post, ~addPostLikeCB, ~removePostLikeCB) => {
  let (saving, setSaving) = React.useState(() => false);
  let liked = Post.likedByUser(post);
  let tip =
    <div className="text-center"> {t("like_button_tooltip") |> str} </div>;
  <div className="flex flex-row-reverse md:flex-row">
    <div className="text-center pr-4 md:pt-4">
      <Tooltip tip position=`Top>
        <button
          ariaLabel={
            (
              if (liked) {
                t("unlike");
              } else {
                t("like");
              }
            )
            ++ ((" " ++ t("post")) ++ " ")
            ++ Post.id(post)
          }
          className="cursor-pointer flex md:flex-col items-center"
          onClick={handlePostLike(
            saving,
            liked,
            setSaving,
            Post.id(post),
            removePostLikeCB,
            handleCreateResponse,
            addPostLikeCB,
          )}>
          <div
            className="flex items-center justify-center rounded-lg lg:rounded-full lg:bg-gray-50 hover:bg-gray-300 text-gray-600 hover:text-gray-900 h-8 w-8 md:h-10 md:w-10 p-1 md:p-2 mx-auto"
            key={iconClasses(liked, saving)}>
            <i className={iconClasses(liked, saving)} />
          </div>
          <p className="text-tiny lg:text-xs font-semibold">
            {post |> Post.totalLikes |> string_of_int |> str}
          </p>
        </button>
      </Tooltip>
    </div>
  </div>;
};

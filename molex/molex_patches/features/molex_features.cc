#include "base/feature_list.h"
#include "build/build_config.h"

namespace features {

BASE_FEATURE(kMolexM3Omnibox,
             "MolexM3Omnibox",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kMolexM3Tabs,
             "MolexM3Tabs",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kMolexM3Menu,
             "MolexM3Menu",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kMolexM3Bookmarks,
             "MolexM3Bookmarks",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kMolexDynamicColor,
             "MolexDynamicColor",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kMolexM3Settings,
             "MolexM3Settings",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kMolexCast,
             "MolexCast",
             base::FEATURE_ENABLED_BY_DEFAULT);

}
